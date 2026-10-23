/*
 * A036665  Decimal expansion of 1/9801980198019801.
 * https://oeis.org/A036665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod D)) / D)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where D = 9801980198019801.  This gives the n‑th decimal digit of 1/D,  a(0) being the first
   digit after the decimal point. */
#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint64_t D = 9801980198019801ULL;
    uint64_t rem = 1ULL;
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10ULL) % D;
    }
    uint64_t digit = (rem * 10ULL) / D;
    return (seq_t)digit;
}
