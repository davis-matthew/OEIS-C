/*
 * A167531  a(n) = Sum_{d divides n} d*(n/d)^(d-1).
 * https://oeis.org/A167531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*(n/d)^(d-1)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 233, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)  /* offset is 1, but be defensive */
        return 0;

    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t base1 = (seq_t)(n / d1);          /* equals d2 */
            seq_t term1 = (seq_t)d1 * big_pow(base1, (seq_t)(d1 - 1));
            sum += term1;

            if (d2 != d1) {
                seq_t base2 = (seq_t)(n / d2);      /* equals d1 */
                seq_t term2 = (seq_t)d2 * big_pow(base2, (seq_t)(d2 - 1));
                sum += term2;
            }
        }
    }
    return sum;
}
