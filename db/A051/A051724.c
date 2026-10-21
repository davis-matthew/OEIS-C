/*
 * A051724  Numerator of n/12.
 * https://oeis.org/A051724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n/gcd(n,12)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Compute gcd of two 64‑bit unsigned integers */

/* OEIS A051724: numerator of n/12 */
seq_t a(idx_t n)
{
    uint64_t g = gcd_uint64((uint64_t)n, 12ULL);
    return (seq_t)((uint64_t)n / g);
}
