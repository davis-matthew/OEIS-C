/*
 * A208645  Least x>0 such that x^2+x+n is not prime.
 * https://oeis.org/A208645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least x>0 such that x^2 + x + n is not prime (OEIS A208645) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    for (idx_t x = 1; ; ++x) {
        seq_t xx = (seq_t)x;
        seq_t val = xx * xx + xx + n_s;
        if (!is_prime_big(val))
            return xx;
    }
}
