/*
 * A218126  Number of 9-ary sequences with primitive period n.
 * https://oeis.org/A218126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 9^d * mu(n/d) for n>0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 */
            seq_t mu1 = mobius(d2);            /* mu(n/d1) */
            if (mu1 == 1) {
                result += big_pow((seq_t)9, (seq_t)d1);
            } else if (mu1 == -1) {
                result -= big_pow((seq_t)9, (seq_t)d1);
            }

            /* divisor d2 (if distinct) */
            if (d1 != d2) {
                seq_t mu2 = mobius(d1);        /* mu(n/d2) */
                if (mu2 == 1) {
                    result += big_pow((seq_t)9, (seq_t)d2);
                } else if (mu2 == -1) {
                    result -= big_pow((seq_t)9, (seq_t)d2);
                }
            }
        }
    }

    return result;
}
