/*
 * A197083  Number of solutions to a+b+c = d+e+f with 0 < a <= n, 0 <=
 *          b,c,d,e,f <= n.
 * https://oeis.org/A197083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(66*n^3 + 209*n^2 + 231*n + 94) / 120 (valid for
 *            all n >= 0; integer division is exact)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t t1 = (seq_t)n;           /* n            */
    seq_t t2 = t1 + 1;             /* n + 1        */

    /* Compute the cubic polynomial 66*n^3 + 209*n^2 + 231*n + 94 */
    seq_t t1_sq = t1 * t1;                     /* n^2          */
    seq_t t1_cu = t1_sq * t1;                  /* n^3          */
    seq_t t3 = 66 * t1_cu;                     /* 66*n^3       */
    t3 += 209 * t1_sq;                         /* +209*n^2      */
    t3 += 231 * t1;                            /* +231*n        */
    t3 += 94;                                   /* +94           */

    /* Divide by the constituent factors of 120 = 2^3 * 3 * 5 */
    /* Three times division by 2 */
    for (int i = 0; i < 3; ++i) {
        if ((t1 & 1) == 0) {
            t1 /= 2;
        } else if ((t2 & 1) == 0) {
            t2 /= 2;
        } else {
            t3 /= 2;
        }
    }
    /* Division by 3 */
    if (t1 % 3 == 0) {
        t1 /= 3;
    } else if (t2 % 3 == 0) {
        t2 /= 3;
    } else {
        t3 /= 3;
    }
    /* Division by 5 */
    if (t1 % 5 == 0) {
        t1 /= 5;
    } else if (t2 % 5 == 0) {
        t2 /= 5;
    } else {
        t3 /= 5;
    }

    /* Final multiplication */
    seq_t res = t1 * t2;
    res *= t3;

    return res;
}
