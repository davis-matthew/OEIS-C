/*
 * A121242  Number of 2's in first n primes.
 * https://oeis.org/A121242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits '2' in the first n primes
 * Range:     u8 n <= 585, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range → 0 */

    /* Get the n-th prime; this will be the sieve limit. */
    uint32_t pn32 = get_prime(n);          /* n ≥ 1 */
    idx_t limit = (idx_t)pn32;

    uint64_t *primes = NULL;
    size_t count = 0;
    build_primes_upto(limit, &primes, &count);

    seq_t total = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t p = primes[i];
        while (p > 0) {
            if ((p % 10ULL) == 2ULL)
                total++;
            p /= 10ULL;
        }
    }

    free(primes);
    return total;
}
