/*
 * A059844  a(n) = smallest nonzero square x^2 such that n+x^2 is prime.
 * https://oeis.org/A059844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest nonzero square x^2 such that n + x^2 is prime
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t x = 1;
    seq_t sq, val;

    for (;; ++x) {
        sq = x * x;
        val = N + sq;
        if (is_prime_big(val))
            return sq;
    }
}
