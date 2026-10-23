/*
 * A018462  Divisors of 450.
 * https://oeis.org/A018462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth divisor of 450 in increasing order. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[18];

    if (!initialized) {
        size_t pos = 0;
        for (idx_t e2 = 0; e2 <= 1; ++e2) {
            seq_t v2 = 1;
            for (idx_t i = 0; i < e2; ++i) v2 *= 2;
            for (idx_t e3 = 0; e3 <= 2; ++e3) {
                seq_t v3 = v2;
                for (idx_t i = 0; i < e3; ++i) v3 *= 3;
                for (idx_t e5 = 0; e5 <= 2; ++e5) {
                    seq_t v = v3;
                    for (idx_t i = 0; i < e5; ++i) v *= 5;
                    divisors[pos++] = v;
                }
            }
        }
        /* simple insertion sort */
        for (size_t i = 1; i < 18; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n >= 1 && n <= 18)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0;
}
