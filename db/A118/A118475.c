/*
 * A118475  Write numbers from n down to 1 in decreasing order, then move the
 *          1 to the front.
 * https://oeis.org/A118475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = 1 followed by the decimal representations of n, n-1, ...,
 *            2 in that order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned char digit_count(idx_t v) {
    unsigned char d = 0;
    do {
        ++d;
        v /= 10;
    } while (v);
    return d;
}

seq_t a(idx_t n) {
    seq_t result = 1;           /* the leading 1 */
    if (n >= 2) {
        for (idx_t k = n; k >= 2; --k) {
            unsigned char d = digit_count(k);
            seq_t mult = 1;
            for (unsigned char i = 0; i < d; ++i)
                mult *= (seq_t)10;
            result = result * mult + (seq_t)k;
        }
    }
    return result;
}
