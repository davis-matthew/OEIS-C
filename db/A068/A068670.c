/*
 * A068670  Number of digits in the concatenation of first n primes.
 * https://oeis.org/A068670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = Sum_{i=1..n} digit_count(prime_i) (number of decimal
 *            digits of the i-th prime)
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sieve_first_n_primes.h"
#include "../utils/digit_count.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    uint32_t *primes = NULL;
    size_t count = 0;
    sieve_first_n_primes(n, &primes, &count);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < count; ++i) {
        sum += (seq_t)digit_count((uint64_t)primes[i]);
    }

    free(primes);
    return sum;
}
