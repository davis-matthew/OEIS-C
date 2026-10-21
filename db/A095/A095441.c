/*
 * A095441  a(n) = 23 written in base 11 - n.
 * https://oeis.org/A095441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 23 written in base (11 - n)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n > 10 the base 11‑n is ≤0; the sequence is defined only up to n = 10. */
    if (n > 10) return 0;

    uint64_t base = 11 - n;
    seq_t result = 0;

    if (base == 1) {                /* Unary representation: 23 ones */
        for (idx_t i = 0; i < 23; ++i) {
            result = result * (seq_t)10 + 1;
        }
    } else {                        /* Base conversion for 23 in base 'base' */
        uint64_t v = 23;
        uint8_t digits[8];          /* 23 in binary needs at most 5 digits */
        int len = 0;
        while (v > 0) {
            digits[len++] = v % base;
            v /= base;
        }
        if (len == 0) digits[len++] = 0;   /* Handle v == 0 */

        for (int i = len - 1; i >= 0; --i) {
            result = result * (seq_t)10 + digits[i];
        }
    }

    return result;
}
