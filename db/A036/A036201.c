/*
 * A036201  Log base 3 (n) mod 199.
 * https://oeis.org/A036201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 3 of n modulo 199. For n % 199 ==
 *            0 the value returned is 0
 * Range:     u8 n <= 198, u16 n <= 198, u32 n <= 198, u64 n <= 198,
 *            u128 n <= 198, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static uint8_t logtbl[199];
    if (!init) {
        init = true;
        uint8_t val = 1;
        for (uint16_t e = 0; e < 198; ++e) {
            logtbl[val] = (uint8_t)e;
            val = (uint8_t)((val * 3) % 199);
        }
    }
    idx_t r = n % 199;
    if (r == 0)
        return (seq_t)0;
    return (seq_t)logtbl[(uint8_t)r];
}
