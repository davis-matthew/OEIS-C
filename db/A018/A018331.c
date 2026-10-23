/*
 * A018331  Divisors of 198.
 * https://oeis.org/A018331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 198 in ascending order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
        static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        size_t idx = 0;
        for (size_t e2 = 0; e2 <= 1; ++e2) {
            seq_t pow2 = 1;
            for (size_t i = 0; i < e2; ++i) pow2 *= 2;
            for (size_t e3 = 0; e3 <= 2; ++e3) {
                seq_t pow3 = 1;
                for (size_t i = 0; i < e3; ++i) pow3 *= 3;
                for (size_t e11 = 0; e11 <= 1; ++e11) {
                    seq_t pow11 = 1;
                    for (size_t i = 0; i < e11; ++i) pow11 *= 11;
                    divisors[idx++] = pow2 * pow3 * pow11;
                }
            }
        }

        for (size_t i = 0; i < 12; ++i) {
            for (size_t j = i + 1; j < 12; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    if (n >= 1 && n <= 12)
        return divisors[n - 1];
    return (seq_t)0;
}
