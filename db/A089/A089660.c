/*
 * A089660  a(n) = S1(n,3), where S1(n, t) = Sum_{k=0..n} (k^t * Sum_{j=0..k}
 *          binomial(n,j)).
 * https://oeis.org/A089660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 40*a(n-2) + 80*a(n-3) - 80*a(n-4) +
 *            32*a(n-5), with a(0)=0, a(1)=2, a(2)=35, a(3)=276, a(4)=1522
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 44, u128 n <= 103,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* initial values (will be truncated correctly for very small seq_t types) */
    static const seq_t init[5] = {0, 2, 35, (seq_t)276, (seq_t)1522};

    if (n < 5)               /* return the small pre‑computed terms */
        return init[n];

    /* keep the last five values a[n-5]..a[n-1] */
    seq_t a0 = init[0]; /* a[n-5]   */
    seq_t a1 = init[1]; /* a[n-4]   */
    seq_t a2 = init[2]; /* a[n-3]   */
    seq_t a3 = init[3]; /* a[n-2]   */
    seq_t a4 = init[4]; /* a[n-1]   */

    for (idx_t i = 5; i <= n; ++i) {
        /* a[i] = 10*a4 - 40*a3 + 80*a2 - 80*a1 + 32*a0   */
        seq_t next = mul_small(a4, 10);     /* 10*a[n-1]  */
        next -= mul_small(a3, 40);          /* -40*a[n-2] */
        next += mul_small(a2, 80);          /* +80*a[n-3] */
        next -= mul_small(a1, 80);          /* -80*a[n-4] */
        next += mul_small(a0, 32);          /* +32*a[n-5] */

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
