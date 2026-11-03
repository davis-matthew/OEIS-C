/*
 * A005171  Characteristic function of nonprimes: 0 if n is prime, else 1.
 * https://oeis.org/A005171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - A010051(n) (characteristic function of nonprimes)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9426 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    return is_prime_u64(n) ? 0 : 1;
}
