/*
 * A202301  Next prime after the partial sum of the first n primes.
 * https://oeis.org/A202301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next prime after the partial sum of the first n primes
 * Range:     u8 n <= 13, u16 n <= 155, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += (seq_t)get_prime(i);
    }

    seq_t cand = sum + 1;
    while (!is_prime_big(cand)) {
        cand += 1;
    }
    return cand;
}
