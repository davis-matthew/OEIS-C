/*
 * A101306  a(n) = Sum_{i=1..n} {last digit of prime(i)}.
 * https://oeis.org/A101306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=1..n} (prime(i) mod 10)
 * Range:     u8 n <= 51, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/estimate_prime_upper_bound.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find an upper bound for the n-th prime */
    uint64_t upper = estimate_prime_upper_bound(n);
    if (upper < 2) upper = 2;

    /* Allocate sieve array */
    unsigned char *sieve = (unsigned char *)malloc(upper + 1);
    if (!sieve)
        return (seq_t)0; /* out of memory, graceful fallback */

    /* Initialize sieve: 1 means potential prime */
    for (uint64_t i = 0; i <= upper; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Simple Eratosthenes sieve */
    for (uint64_t i = 2; i * i <= upper; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= upper; j += i)
                sieve[j] = 0;
        }
    }

    /* Accumulate sum of last digits */
    idx_t count = 0;
    seq_t sum = (seq_t)0;
    for (uint64_t i = 2; i <= upper && count < n; ++i) {
        if (sieve[i]) {
            ++count;
            sum += (seq_t)(i % 10);
        }
    }

    free(sieve);
    return sum;
}
