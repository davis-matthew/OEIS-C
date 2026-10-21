/*
 * A053061  a(n) is the decimal concatenation of n and n^2.
 * https://oeis.org/A053061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n*(10^floor(2*log10(n)+1)+ n)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. concatenation of n and n^2 in decimal */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* n as seq_t */
    seq_t sq = nn * nn;            /* n^2 */

    /* compute number of decimal digits in sq */
    seq_t tmp = sq;
    unsigned d = 0;
    do
    {
        tmp /= 10;
        d++;
    } while (tmp > 0);

    /* compute 10^d */
    seq_t pow10 = 1;
    for (unsigned i = 0; i < d; ++i)
        pow10 *= 10;

    /* concatenate: n*10^d + n^2 */
    return nn * pow10 + sq;
}
