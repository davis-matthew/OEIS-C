/*
 * A118650  Center column a(2n,n) of A106597.
 * https://oeis.org/A118650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a recurrence valid for n ≥ 4 * n*a(n) + 3*(-3*n+2)*a(n-1) +
 *            4*(5*n-8)*a(n-2) + 2*(-8*n+21)*a(n-3) * + 2*(2*n-7)*a(n-4) = 0 *
 *            with initial values * a(0)=1, a(1)=3, a(2)=14, a(3)=72. * The
 *            implementation computes the terms iteratively using a ring
 *            buffer of * the last four values, avoiding large al...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)72;

    /* Ring buffer for the last four values: indices modulo 4. */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0)   */
    buf[1] = (seq_t)3;   /* a(1)   */
    buf[2] = (seq_t)14;  /* a(2)   */
    buf[3] = (seq_t)72;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        size_t im1 = (i - 1) & 3;
        size_t im2 = (i - 2) & 3;
        size_t im3 = (i - 3) & 3;
        size_t im4 = (i - 4) & 3;
        size_t idx  = i & 3; /* where to store a(i) */

        /* Compute the terms needed for the recurrence expressed with
         * only positive coefficients:
         *   a(i) = ( 3*(3*i-2)*a(i-1)
         *          - 4*(5*i-8)*a(i-2)
         *          + 2*(8*i-21)*a(i-3)
         *          - 2*(2*i-7)*a(i-4) ) / i
         */
        seq_t termA = (seq_t)3 * (seq_t)(3*i - 2) * buf[im1];
        seq_t termB = (seq_t)4 * (seq_t)(5*i - 8) * buf[im2];
        seq_t termC = (seq_t)2 * (seq_t)(8*i - 21) * buf[im3];
        seq_t termD = (seq_t)2 * (seq_t)(2*i - 7) * buf[im4];

        seq_t numerator = termA + termC;
        numerator -= termB;  /* safe: the recurrence guarantees termA + termC ≥ termB */
        numerator -= termD;  /* safe: after previous subtraction, the result remains ≥ 0 */

        buf[idx] = numerator / (seq_t)i;
    }

    return buf[n & 3];
}
