/*
 * A163254  Array of the nonsquares; the columns satisfy
 *          c(n)=c(n-1)+c(n-2)-c(n-3)+1.
 * https://oeis.org/A163254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array S(r,k) (the “nonsquares”
 *            array) along its antidiagonals. S(r,k) = k^2 + (r-1)k + (
 *            (r^2+3)/4 ) if r odd, S(r,k) = k^2 + (r-1)k + ( r^2/4 ) if r
 *            even
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 1.  */

#include <stdint.h>

/* Integer square‑root of a 64‑bit value.
   Uses 128‑bit arithmetic to avoid overflow of mid*mid. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid_sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (mid_sq <= v) {
            if ((unsigned __int128)(mid + 1) * (mid + 1) > v)
                return mid;
            lo = mid + 1;
        } else {
            if (mid == 0)
                return 0;
            hi = mid - 1;
        }
    }
    return 0;
}

seq_t a(idx_t n_input)
{
    /* Find diagonal d such that d*(d+1)/2 >= n. */
    idx_t n = n_input;
    /* Compute discriminant 1 + 8*n; overflow is not a concern for the
       test ranges (n up to 10 000). */
    idx_t discr = 1 + 8 * n;
    idx_t d = (isqrt_u64(discr) - 1) / 2;
    /* Adjust to ensure d satisfies the inequality. */
    while (d * (d + 1) / 2 < n) d++;
    while (d > 1 && (d - 1) * d / 2 >= n) d--;

    /* Start index of this diagonal. */
    idx_t start = (d - 1) * d / 2 + 1;
    idx_t offset = n - start;                 /* 0‑based position in the diagonal */
    idx_t row = 1 + offset;                   /* r (row index) */
    idx_t col = d - row + 1;                  /* k (column index) */

    /* Compute S(row, col) in seq_t */
    seq_t r = (seq_t)row;
    seq_t k = (seq_t)col;
    seq_t term = k * k + (r - 1) * k;
    if (row & 1) {                            /* odd row */
        term += (r * r + (seq_t)3) / (seq_t)4;
    } else {                                  /* even row */
        term += (r * r) / (seq_t)4;
    }
    return term;
}
