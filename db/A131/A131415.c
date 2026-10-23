/*
 * A131415  (A007318 * A000012) + (A000012 * A007318) - A007318.
 * https://oeis.org/A131415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (A007318 * A000012)(i,j) + (A000012 * A007318)(i,j) -
 *            A007318(i,j) where A007318 is Pascal’s triangle (binomial
 *            coefficients) and A000012 is a lower triangular matrix of ones.
 *            The triangle entries are t(i,j) = sum_{k=j}^{i} C(i,k) +
 *            sum_{k=j}^{i} C(k,j) - C(i,j) with 0 ≤ j ≤ i. The sequenc...
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for idx_t and seq_t are provided by the build system. */
#include <stdint.h>

/* Return the nth term of the sequence.  The function is thread‑safe and
   works for any n >= 0. */
seq_t a(idx_t n)
{
    /* Find the row index i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2.  */
    idx_t low  = 0ULL;
    idx_t high = ((idx_t)1 << 32) - 1ULL;      /* > sqrt(2*UINT64_MAX)   */
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        /* mid*(mid+1)/2 fits into 64 bits because mid < 2^32. */
        idx_t triangular = (mid & 1ULL) ?
            ((mid + 1) / 2ULL) * mid                             :
            (mid / 2ULL) * (mid + 1);
        if (triangular <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t i = high;
    idx_t triangular = (i & 1ULL) ?
        ((i + 1) / 2ULL) * i :
        (i / 2ULL) * (i + 1);
    idx_t j = n - triangular;          /* column index (0 ≤ j ≤ i) */

    /* ----- Compute the triangle entry t(i,j) ----- */
    seq_t sum_row = 0;
    seq_t sum_col = 0;
    seq_t bin_row = 1;         /* C(i,0) */
    seq_t bin_col = 1;         /* will be updated for k=j .. i */
    seq_t binij   = 1;         /* C(i,j) */

    /* Row sums and the binomial C(i,j) */
    for (idx_t k = 0; k <= i; ++k)
    {
        if (k >= j)
        {
            sum_row += bin_row;
            if (k == j)
                binij = bin_row;
        }
        if (k < i)
            bin_row = bin_row * (seq_t)(i - k) / (seq_t)(k + 1);
    }

    /* Column sums */
    bin_col = 1;                /* C(j,j) */
    for (idx_t k = j; k <= i; ++k)
    {
        if (k > j)
            bin_col = bin_col * (seq_t)k / (seq_t)(k - j);
        sum_col += bin_col;
    }

    return sum_row + sum_col - binij;
}
