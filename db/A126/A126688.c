/*
 * A126688  Lowest base in which n has distinct digits.
 * https://oeis.org/A126688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ b>=2 | representation of n in base b uses all
 *            distinct digits }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* At least one digit is needed; n >= 1 by definition of the sequence. */
    for (idx_t base = 2; base <= n + 1; ++base) {
        idx_t val = n;
        /* Maximum number of digits occurs when base = 2.
           For uint64_t n the number of bits is <= 64, so 128 is safe. */
        idx_t digits[128];
        size_t len = 0;

        /* Convert to base 'base' and collect the digits. */
        while (val > 0) {
            digits[len++] = val % base;
            val /= base;
        }

        /* Check that all digits are distinct. */
        bool ok = true;
        for (size_t i = 0; i < len && ok; ++i) {
            for (size_t j = i + 1; j < len; ++j) {
                if (digits[i] == digits[j]) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            return (seq_t)base;
        }
    }

    /* In theory we never reach this point because n+1 always works
       (for base > n the representation is a single digit). */
    return (seq_t)(n + 1);
}
