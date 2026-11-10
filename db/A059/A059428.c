/*
 * A059428  Number of points of rotation in a prime block spiral.
 * https://oeis.org/A059428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(A002620(n)+1), where A002620(n) =
 *            floor(n/2)*ceiling(n/2)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

static idx_t A002620_a(idx_t n) {
    return (n >> 1) * ((n + 1) >> 1);
}

seq_t a(idx_t n) {
    idx_t m = A002620_a(n) + 1;
    return (seq_t)nth_prime(m);
}
