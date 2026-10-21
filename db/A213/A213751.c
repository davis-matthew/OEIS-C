/*
 * A213751  Rectangular array: (row n) = b**c, where b(h) = 2*h-1, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A213751: a(n) is the nth element of the rectangular array
   defined by T(r,c) = Σ_{h=1..c} (2h-1)*(r+c-h), read by falling
   antidiagonals.  For each index n >= 1, the a(n) value is
   obtained by first locating the corresponding (row, col) pair,
   then evaluating the convolution formula above.  The algorithm
   runs in O(√n) time and uses only integer arithmetic on seq_t.
*/

#include <stdint.h>   /* for uint64_t, etc. */

static inline seq_t compute_T(idx_t r, idx_t c)
{
    /* r = row >=1, c = col >=1 */
    seq_t k = (seq_t)c;
    seq_t n = (seq_t)r;

    /* k^2*(n + k) */
    seq_t k2 = k * k;
    seq_t t1 = k2 * (n + k);

    /* k(k+1)(2k+1)/3 */
    seq_t t2 = k * (k + 1) * (2 * k + 1);
    t2 /= 3;

    /* k(k+1)/2 */
    seq_t t3 = k * (k + 1) / 2;

    return t1 - t2 + t3;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal (sum = s) containing index n.
       Number of terms up to sum s is s*(s-1)/2.  */
    idx_t s = 1;
    while (s * (s - 1) / 2 < n)
        ++s;

    /* Index within this diagonal (1-based) */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t pos  = n - prev;           /* 1 <= pos <= s-1 */

    idx_t row = pos;          /* i */
    idx_t col = s - pos;      /* j */

    return compute_T(row, col);
}
