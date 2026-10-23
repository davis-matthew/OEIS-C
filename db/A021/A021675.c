/*
 * A021675  Decimal expansion of 1/671.
 * https://oeis.org/A021675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10*(10^n mod 671)/671)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/671 (offset 0). */
#include <stdint.h>

static inline uint64_t modpow10(uint64_t exp) {
    uint64_t result = 1;
    uint64_t base = 10;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % 671ULL;
        base = (base * base) % 671ULL;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^n mod 671 */
    uint64_t r = modpow10((uint64_t)n);
    /* digit = floor(10 * r / 671) ; r < 671, so 10*r < 6710 fits in 64 bits */
    return (seq_t)((r * 10ULL) / 671ULL);
}
