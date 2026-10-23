/*
 * A037034  Least k such that 4*n*k-1 is a prime.
 * https://oeis.org/A037034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* a(n) is the least k such that 4*n*k-1 is prime */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    for (idx_t k = 1;; ++k) {
        seq_t x = (seq_t)4 * nseq * (seq_t)k - 1;
        if (is_prime_big(x))
            return (seq_t)k;
    }
}
