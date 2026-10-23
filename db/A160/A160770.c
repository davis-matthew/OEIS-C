/*
 * A160770  Triangle in which row n is generated from (1,3,6,10,...n) dot
 *          (n,n-1,...1) with subtractive carryovers.
 * https://oeis.org/A160770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* The sequence A160770 is defined by the subtractive carryover
 * dot product of the two vectors:
 *
 *   A[i] = triangular(i+1)   (i = 0 .. r)
 *   B[i] = triangular(r+1-i) (i = 0 .. r)
 *
 * with
 *   c[0] = A[0] * B[0]
 *   c[i] = A[i] * B[i] - c[i-1]   (i > 0)
 *
 * The rows of the triangle are listed row‑wise (row 0, then row 1, …)
 * starting at offset 0.  For an index n we find the unique row
 * r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 and set c = n - r(r+1)/2.
 * The desired value a(n) is the c‑th element of the row r, i.e. c(c) .
 */

/* triangular number T(k) = k*(k+1)/2, computed in seq_t */
static inline seq_t triangular(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

seq_t a(idx_t n)
{
    /* determine row r and column c for the given index n */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t c = n - r * (r + 1) / 2;

    seq_t prev = 0;
    seq_t cur  = 0;
    for (idx_t i = 0; i <= c; ++i)
    {
        seq_t a_i = triangular(i + 1);          /* triangular(i+1)   */
        seq_t b_i = triangular(r + 1 - i);      /* triangular(r+1-i) */
        if (i == 0)
            cur = a_i * b_i;
        else
            cur = a_i * b_i - prev;
        prev = cur;
    }
    return cur;
}
