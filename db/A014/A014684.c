/*
 * A014684  In the sequence of positive integers subtract 1 from each prime
 *          number.
 * https://oeis.org/A014684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - A010051(n) (subtract 1 from prime numbers)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9625 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (is_prime(n))
        return (seq_t)(n - 1);
    else
        return (seq_t)n;
}
