/*
 * A021470  Decimal expansion of 1/466.
 * https://oeis.org/A021470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/466) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    static unsigned char period[232];
    static int initialized = 0;
    static const idx_t period_len = 232;
    if (!initialized) {
        uint64_t r = 1;
        for (idx_t i = 0; i < period_len; ++i) {
            r *= 10;
            period[i] = (unsigned char)(r / 466);
            r %= 466;
        }
        initialized = 1;
    }
    return (seq_t)period[n % period_len];
}
