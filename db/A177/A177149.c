/*
 * A177149  Indices n such that the sums of the squares of the digits of
 *          prime(n) are prime.
 * https://oeis.org/A177149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_square_sum, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th natural number m such that
   the sum of squares of the digits of the m‑th prime is prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_square_sum.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t m = 1;
    idx_t found = 0;
    while (1) {
        uint32_t p = get_prime(m);
        uint64_t s = digit_square_sum(p);
        if (is_prime_small((seq_t)s)) {
            ++found;
            if (found == n) return (seq_t)m;
        }
        ++m;
    }
}
