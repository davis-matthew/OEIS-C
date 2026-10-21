/*
 * A059324  Numbers n such that 6n + 5 is composite.
 * https://oeis.org/A059324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers n such that 6*n+5 is composite. Linear search using primality test. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 1;
    while (1) {
        /* compute 6*m + 5 in seq_t to test for primality */
        seq_t v = (seq_t)m;
        v *= 6;
        v += 5;
        if (!is_prime_big(v)) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
