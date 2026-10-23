/*
 * A011602  Legendre symbol (n,97).
 * https://oeis.org/A011602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,97) a(0) = 0 a(n) = 1 if n^48 (mod 97)
 *            = 1 a(n) = -1 otherwise a(n) = 0 if n mod 97 = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t lut[97];
    static bool initialized = false;

    if (!initialized) {
        for (uint64_t r = 0; r < 97; ++r) {
            if (r == 0) {
                lut[(int) r] = 0;
                continue;
            }

            uint64_t base = r % 97;
            uint64_t exp  = 48;
            uint64_t result = 1;

            /* fast modular exponentiation, all intermediates fit in uint64_t */
            while (exp) {
                if (exp & 1)
                    result = (result * base) % 97;
                base = (base * base) % 97;
                exp >>= 1;
            }
            lut[(int) r] = (result == 1) ? 1 : -1;
        }
        initialized = true;
    }

    return lut[(int)(n % 97)];
}
