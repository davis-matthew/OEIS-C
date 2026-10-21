/*
 * A065897  The a(n)-th composite number is twice the n-th prime.
 * https://oeis.org/A065897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n) - pi(2*prime(n)) - 1
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a simple sieve to produce primes up to 2*prime(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* n >= 1 is expected by usage. For safety, handle n==0. */
    if (n == 0) return 0;

    /* Estimate an upper bound for the n-th prime. */
    idx_t prime_est = _prime_limit_for_idx(n);
    /* Ensure we sieve far enough for both the n-th prime and 2*prime(n). */
    idx_t limit = prime_est * 2 + 10;

    /* Allocate array for primes and get their count. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Safety: if sieve failed or not enough primes, return 0. */
    if (!primes || prime_cnt < n) {
        if (primes) free(primes);
        return 0;
    }

    /* Value of the n-th prime (0‑based index). */
    seq_t prime_n = primes[n - 1];

    /* Count primes <= 2*prime_n using binary search. */
    idx_t upper = (idx_t)prime_n * 2;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= (uint64_t)upper)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t pi_val = lo; /* number of primes <= upper */

    free(primes);

    /* a(n) = 2*prime(n) - pi(2*prime(n)) - 1 */
    seq_t result = ((seq_t)prime_n << 1) - (seq_t)pi_val - 1;
    return result;
}
