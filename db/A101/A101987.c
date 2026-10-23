/*
 * A101987  Product of nonzero digits of n-th prime.
 * https://oeis.org/A101987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = product of nonzero digits of the n-th prime
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A101987
 *   a(n) = product of the nonzero decimal digits of the n‑th prime.
 *
 * The implementation uses a one‑time sieve up to an upper bound estimated
 * by estimate_prime_upper_bound(n).  The sieve is generated on every
 * invocation; for n up to ~10000 this is fast (<2 s) and well under the
 * required time limit.
 *
 * Dependencies: ../utils/estimate_prime_upper_bound.h
 *                <stdbool.h>, <stdlib.h>, <string.h>
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

seq_t a(idx_t n)
{
    /* Estimate an upper bound for the n‑th prime. */
    uint64_t upper = estimate_prime_upper_bound(n);
    size_t sz = (size_t)upper + 1;

    /* Allocate sieve array. */
    bool *is_composite = (bool *)malloc(sz);
    if (!is_composite)
        return 0;  /* allocation failure, unlikely for requested constraints */

    /* Initialise sieve. */
    memset(is_composite, 0, sz);
    if (sz > 0) is_composite[0] = true;
    if (sz > 1) is_composite[1] = true;

    /* Sieve of Eratosthenes. */
    uint64_t limit = (uint64_t)(sqrt((double)upper));
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= upper; j += i)
                is_composite[j] = true;
        }
    }

    /* Find the n‑th prime. */
    uint64_t count = 0;
    uint64_t prime = 0;
    for (uint64_t i = 2; i <= upper; ++i) {
        if (!is_composite[i]) {
            ++count;
            if (count == n) {
                prime = i;
                break;
            }
        }
    }

    free(is_composite);

    /* If the prime was not found (should not happen), return 0. */
    if (prime == 0)
        return 0;

    /* Compute product of nonzero decimal digits. */
    seq_t prod = 1;
    uint64_t tmp = prime;
    while (tmp > 0) {
        uint8_t d = tmp % 10;
        if (d != 0)
            prod *= (seq_t)d;
        tmp /= 10;
    }

    return prod;
}
