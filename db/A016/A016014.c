/*
 * A016014  Least k such that 2*n*k + 1 is a prime.
 * https://oeis.org/A016014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   least k such that 2*n*k+1 is prime
 * Range:     u8 n <= 30, u16 n <= 5166, u32 n <= 5166, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 2*n must be computed as seq_t to avoid overflow */
    seq_t two_n = (seq_t)n * (seq_t)2;
    seq_t k = 1;
    for (;; ++k) {
        seq_t candidate = two_n * k + (seq_t)1;
        if (is_prime_big(candidate))
            return k;
    }
}
