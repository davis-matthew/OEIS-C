/*
 * A134402  Triangle read by rows, for n > 0, n zeros followed by n.
 * https://oeis.org/A134402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if n==0; otherwise a(n) = k where k = (sqrt(8*n+9)-3)/2
 *            if 8*n+9 is a perfect square and the result is integer, else
 *            a(n)=0
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t x  = 9 + (seq_t)8 * nn;      /* x = 8*n + 9 */
    seq_t t  = floor_sqrt(x);          /* t = floor(sqrt(x)) */

    /* check if x is a perfect square and t is odd (so t-3 is even) */
    if (t * t != x) return (seq_t)0;
    if ((t & 1U) == 0) return (seq_t)0;   /* t must be odd */

    seq_t k = (t - (seq_t)3) / (seq_t)2;   /* k = (t-3)/2 */

    /* verify that k satisfies k*(k+3)/2 == n */
    if (k * (k + (seq_t)3) / (seq_t)2 == nn)
        return k;
    return (seq_t)0;
}
