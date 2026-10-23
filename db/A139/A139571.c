/*
 * A139571  Numbers with 31 divisors.
 * https://oeis.org/A139571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^30
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);
    return pow_uint((uint64_t)p, 30);
}
