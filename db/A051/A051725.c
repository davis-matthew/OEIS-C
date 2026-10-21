/*
 * A051725  Denominator of n/12.
 * https://oeis.org/A051725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 12 / gcd(n,12) , with a(0)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for unsigned 64‑bit integers */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0ULL) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t g = gcd_uint64((uint64_t)n, 12ULL);
    return (seq_t)(12ULL / g);
}
