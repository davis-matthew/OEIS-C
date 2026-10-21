/*
 * A060091  Number of 4-block ordered bicoverings of an unlabeled n-set.
 * https://oeis.org/A060091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) - 4*binomial(n+2,2) - 3*binomial(n+1,1) +
 *            12, n>=0
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 192, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;                     /* a(0)=a(1)=a(2)=0 */

    seq_t res = binomial(n + 5, 5);           /* C(n+5,5) */
    res -= 4 * binomial(n + 2, 2);            /* -4*C(n+2,2) */
    res -= 3 * binomial(n + 1, 1);            /* -3*C(n+1,1) */
    res += 12;                                /* +12*1  (C(n,0)=1) */

    return res;                               /* a(n) for n>=3 */
}
