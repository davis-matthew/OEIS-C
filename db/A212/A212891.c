/*
 * A212891  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          (n-1+h)^2, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A212891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212891
   Rectangular array: (row n) = b**c,
   where b(h) = h, c(h) = (n-1+h)^2, n≥1, h≥1,
   and ** = convolution.
   a(n) is the terms read along falling antidiagonals of this array.
   Implementation: compute the position (r,c) corresponding to n,
   then evaluate the convolution sum directly.
*/
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the smallest s such that s*(s-1)/2 >= n.
       s is the sum index of the antidiagonal. */
    idx_t s = 1;
    while ((s * (s - 1) / 2) < n) {
        ++s;
    }

    /* Number of elements before this diagonal:
       f(s-1) = (s-2)*(s-1)/2. */
    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev;            /* 1‑based offset within diagonal s */

    idx_t r = offset;                    /* row index (1‑based)   */
    idx_t c = s - offset;                /* column index (1‑based) */

    /* Evaluate T(r,c) = sum_{h=1}^c h * ((r + c - h)^2) */
    seq_t sum = 0;
    seq_t r_seq = (seq_t)r;
    seq_t c_seq = (seq_t)c;
    for (idx_t h = 1; h <= c; ++h) {
        seq_t h_seq = (seq_t)h;
        seq_t t = r_seq + c_seq - h_seq;       /* t = r + c - h */
        sum += h_seq * t * t;
    }
    return sum;
}
