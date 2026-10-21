/*
 * A083899  Number of divisors of n with largest digit <= 4 (base 10).
 * https://oeis.org/A083899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   small_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of divisors of n whose largest decimal digit is <= 4. */
#include <stdint.h>
#include "../utils/small_digits.h"

/* seq_t a(idx_t n) – see prelude for type definitions. */
seq_t a(idx_t n) {
    /* Ensure n is unsigned 64‑bit; the prelude may define idx_t differently. */
    uint64_t nn = (uint64_t)n;
    seq_t count = 0;

    for (uint64_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            /* divisor i */
            if (small_digits(i)) {
                count++;
            }
            /* complementary divisor nn / i, avoid double counting when i == nn / i */
            uint64_t j = nn / i;
            if (j != i && small_digits(j)) {
                count++;
            }
        }
    }
    return count;
}
