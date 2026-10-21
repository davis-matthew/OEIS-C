/*
 * A220188  Number of representations of n-th prime as 2p + 3q (p, q primes
 *          not necessarily distinct).
 * https://oeis.org/A220188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of the n-th prime as 2p + 3q
 *            with p and q primes (not necessarily distinct). The function
 *            builds the list of all primes up to the n-th prime and counts
 *            all pairs (p,q) that satisfy 2p+3q = nth_prime
 * Range:     u8 n <= 4390, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* Get the n‑th prime (1‑indexed) */
    uint32_t nth_prime = get_prime(n);

    /* Generate all primes ≤ nth_prime */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)nth_prime, &primes, &prime_cnt);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > nth_prime / 2) break;
        uint64_t residual = (uint64_t)nth_prime - 2ULL * p;
        if (residual % 3ULL != 0ULL) continue;
        uint64_t q = residual / 3ULL;
        if (q < 2ULL) continue;
        if (is_prime((idx_t)q)) {
            ++count;
        }
    }

    free(primes);
    return count;
}
