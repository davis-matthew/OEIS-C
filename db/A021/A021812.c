/*
 * A021812  Decimal expansion of 1/808.
 * https://oeis.org/A021812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (floor(10^(n+1)/808)) mod 10 – the n-th digit after the
 *            decimal point of 1/808
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   powmod_10_mod_8080
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powmod_10_mod_8080.h"

seq_t a(idx_t n)
{
    uint64_t r = powmod_10_mod_8080((uint64_t)n + 1ULL);
    return (seq_t)(r / 808ULL);       /* r < 8080, division fits in seq_t */
}
