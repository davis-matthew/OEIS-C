/*
 * A060547  a(n) = 2^(floor(n/3) + ((n mod 3) mod 2)).
 * https://oeis.org/A060547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2^(floor(n/3) + ((n mod 3) mod 2))
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 93, u64 n <= 189,
 *            u128 n <= 381, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* exponent = floor(n/3) + ((n mod 3) mod 2) */
    idx_t exp = (n / 3) + ((n % 3) & 1);
    return pow2(exp);
}
