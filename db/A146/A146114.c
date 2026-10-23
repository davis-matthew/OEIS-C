/*
 * A146114  Bell numbers (A000110) read mod 24.
 * https://oeis.org/A146114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) (mod 24) Computation: Bell numbers modulo 24 via
 *            the Bell triangle. Periodicity a(n+312) = a(n) is used to cache
 *            the first 312 terms. Only O(312²) operations are required to
 *            initialize
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t bell[312];
    static bool initialized = false;

    if (!initialized) {
        seq_t prev[312] = {0};
        seq_t curr[312] = {0};

        prev[0] = 1;
        bell[0] = 1;
        for (idx_t row = 1; row <= 311; ++row) {
            curr[0] = prev[row - 1];
            for (idx_t k = 1; k <= row; ++k) {
                seq_t sum = curr[k - 1] + prev[k - 1];
                curr[k] = sum % (seq_t)24;
            }
            bell[row] = curr[0];
            for (idx_t i = 0; i <= row; ++i)
                prev[i] = curr[i];
        }
        initialized = true;
    }

    return bell[n % 312];
}
