/*
 * A118411  Numerator of sum of reciprocals of first n pentatope numbers
 *          A000332.
 * https://oeis.org/A118411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n)/A118412(n) = Sum_{i=1..n} 24/(i*(i+1)*(i+2)*(i+3))
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1057, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    seq_t num = 0;   /* running numerator of the sum (reduced) */
    seq_t den = 1;   /* running denominator of the sum (reduced) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t i = (seq_t)k;
        /* term = 24 / (i*(i+1)*(i+2)*(i+3)) */
        seq_t term_num = (seq_t)24;
        seq_t term_den = i * (i + 1) * (i + 2) * (i + 3);

        /* add term to current fraction num/den, keeping it reduced */
        seq_t g1 = gcd(den, term_den);          /* gcd to compute lcm */
        seq_t lcm = (den / g1) * term_den;      /* lcm = den/g1 * term_den */

        seq_t nnum = num * (term_den / g1) + term_num * (den / g1);
        seq_t g2 = gcd(nnum, lcm);

        num = nnum / g2;
        den = lcm / g2;
    }

    return num;  /* the numerator of the reduced sum */
}
