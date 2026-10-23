/*
 * A018513  Divisors of 549.
 * https://oeis.org/A018513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 549 in ascending order; a(n) = 0 for
 *            n>6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool init = false;

    if (!init) {
        /* 549 = 3^2 * 61^1 */
        const uint64_t primes[2] = {3, 61};
        const uint8_t exps[2]   = {2, 1};
        size_t idx = 0;

        for (uint8_t e1 = 0; e1 <= exps[0]; ++e1) {
            for (uint8_t e2 = 0; e2 <= exps[1]; ++e2) {
                uint64_t v = 1;
                for (uint8_t i = 0; i < e1; ++i) v *= primes[0];
                for (uint8_t i = 0; i < e2; ++i) v *= primes[1];
                divisors[idx++] = (seq_t)v;
            }
        }

        /* sort the 6 divisors in ascending order */
        for (size_t i = 0; i < 6; ++i) {
            for (size_t j = i + 1; j < 6; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        init = true;
    }

    if (n < 1 || n > 6) return 0;
    return divisors[(size_t)(n - 1)];
}
