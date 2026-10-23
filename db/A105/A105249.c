/*
 * A105249  a(n) = binomial(n+2,n)*binomial(n+6,n).
 * https://oeis.org/A105249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+2,n) * binomial(n+6,n) (offset 0)
 * Range:     u8 none, u16 n <= 3, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* binomial(n+2, n) = binomial(n+2, 2) = (n+2)(n+1)/2 */
    seq_t t1 = nn + 2;
    t1 *= (nn + 1);
    t1 /= 2;

    /* binomial(n+6, n) = binomial(n+6, 6) = (n+6)(n+5)(n+4)(n+3)(n+2)(n+1)/720 */
    seq_t t2 = 1;
    t2 *= (nn + 6);
    t2 *= (nn + 5);
    t2 *= (nn + 4);
    t2 *= (nn + 3);
    t2 *= (nn + 2);
    t2 *= (nn + 1);
    t2 /= 720;

    return t1 * t2;
}
