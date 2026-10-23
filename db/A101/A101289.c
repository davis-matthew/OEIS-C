/*
 * A101289  Inverse Moebius transform of 5-simplex numbers A000389.
 * https://oeis.org/A101289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d+4,5)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 217, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_d_plus_4_choose_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_d_plus_4_choose_5.h"

/* Main function required by the build.  It uses only integer arithmetic
 * on seq_t and idx_t and does not depend on any external utilities.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* safety guard – not part of sequence */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            sum += binom_d_plus_4_choose_5(i);
            idx_t j = n / i;
            if (j != i) {
                /* complementary divisor j */
                sum += binom_d_plus_4_choose_5(j);
            }
        }
    }
    return sum;
}
