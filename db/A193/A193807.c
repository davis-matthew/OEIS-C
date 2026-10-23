/*
 * A193807  Smallest positive integer k such that n*k^2 + 1 is a prime.
 * https://oeis.org/A193807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that n*k^2+1 is prime
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 76, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t k = 1;
    for (;;) {
        seq_t val = n_seq * k * k + 1;
        if (is_prime_big(val))
            return k;
        ++k;
    }
}
