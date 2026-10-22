/*
 * A006425  Number of rooted toroidal maps with 2 vertices and n faces and no
 *          isthmuses.
 * https://oeis.org/A006425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n*(n+1)*(n+2)*(7*n+15)/12 -
 *            binomial(2*n,n)*(2*n+1)*(2*n+3)*(3*n+5)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* term1 = 4^(n-1) * (n+1)(n+2)(7n+15)/3 */
    uint64_t f1 = n + 1, f2 = n + 2, f3 = 7 * n + 15;
    if (f1 % 3 == 0) f1 /= 3;
    else if (f2 % 3 == 0) f2 /= 3;
    else f3 /= 3;
    seq_t p4 = 1;
    for (idx_t i = 1; i < n; i++) p4 *= 4;
    seq_t t1 = p4 * (seq_t)f1;
    t1 *= (seq_t)f2;
    t1 *= (seq_t)f3;

    /* c = binomial(2n-1, n-1) = binomial(2n,n)/2 */
    seq_t c = 1;
    for (idx_t i = 1; i < n; i++) {
        c *= (seq_t)(n + i);
        c /= (seq_t)i;
    }

    /* term2 = c * (2n+1)(2n+3)(3n+5)/3 */
    uint64_t g1 = 2 * n + 1, g2 = 2 * n + 3, g3 = 3 * n + 5;
    if (g1 % 3 == 0) g1 /= 3;
    else if (g2 % 3 == 0) g2 /= 3;
    else c /= 3;
    seq_t t2 = c * (seq_t)g1;
    t2 *= (seq_t)g2;
    t2 *= (seq_t)g3;

    return t1 - t2;
}
