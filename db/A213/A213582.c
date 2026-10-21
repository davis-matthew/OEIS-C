/*
 * A213582  Rectangular array: (row n) = b**c, where b(h) = -1 + 2^h, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the rectangular array T(n,k) along
 *            anti‑diagonals, where T(n,k) = 2*(n+1)*(2^k - 1) - k*(k + 2*n +
 *            3)/2 For n>=1, k>=1. The array entries are 1‑based. The sequence
 *            a(n) is the array entry at position (row,column) obtained from
 *            the anti‑diagonal index n
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

/* Compute T(n,k) for 1‑based indices n,k */
static inline seq_t T_val(idx_t n, idx_t k)
{
    seq_t row = (seq_t)n;
    seq_t col = (seq_t)k;
    seq_t pow2k = pow2((idx_t)k);          /* 2^k   */
    seq_t part1 = 2 * (row + 1) * (pow2k - 1);
    seq_t part2 = col * (col + 2*row + 3) / 2;
    return part1 - part2;
}

/* Return the anti‑diagonal index d such that
   d*(d+1)/2 >= n > (d-1)*d/2   */
static inline idx_t diag_from_index(idx_t n)
{
    /* Initial estimate via the quadratic formula */
    double d_est = (sqrt(8.0*n + 1.0) - 1.0) / 2.0;
    idx_t d = (idx_t)d_est;
    /* Adjust upwards if necessary (should be at most 1 step) */
    while ((__int128)d*(d+1)/2 < n) ++d;
    /* Adjust downwards if overshot */
    while (d > 0 && (__int128)(d-1)*d/2 >= n) --d;
    return d;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n<1, but not used */

    /* Determine row i and column j from index n */
    idx_t d        = diag_from_index(n);
    idx_t prev_sum = (idx_t)((__int128)(d-1)*d/2);
    idx_t offset   = n - prev_sum - 1;   /* 0‑based offset on diagonal */
    idx_t i        = offset + 1;         /* row   (1‑based) */
    idx_t j        = d - offset;         /* column(1‑based) */

    return T_val(i, j);
}
