/*
 * A061108  Compute Euler totient function for the prime(n+1)-prime(n)-1
 *          composite numbers between two consecutive primes; choose the
 *          largest.
 * https://oeis.org/A061108
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max{phi(c); prime(n) < c < prime(n+1)}
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, calc_phi,
 *            is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a composite integer c in the prime gap and returns the
 * maximal Euler totient value within that gap. */
/* Standard C11 / C++17 compliant. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"
#include "../utils/calc_phi.h"

static uint64_t *prime_cache = NULL;   /* array of primes, 1-based in OEIS sense */
static size_t prime_cache_cnt = 0;      /* number of primes stored */

/* Ensure that we have at least 'idx' primes available in prime_cache. */
static void ensure_primes_for_idx(idx_t idx)
{
    if (prime_cache_cnt >= idx)
        return; /* already sufficient */

    /* Get an upper bound for the idx-th prime. */
    idx_t limit = _prime_limit_for_idx(idx);
    free(prime_cache);   /* discard old array if any */
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2; can be treated as 0 */

    /* We need prime(n) and prime(n+1), so at least (n+1) primes. */
    ensure_primes_for_idx(n + 1);

    uint64_t prime_n    = prime_cache[n - 1]; /* prime(n) */
    uint64_t prime_next = prime_cache[n];     /* prime(n+1) */

    seq_t max_phi = 0;

    for (uint64_t c = prime_n + 1; c < prime_next; ++c) {
        if (is_prime_small(c))
            continue; /* skip primes */
        seq_t ph = calc_phi((seq_t)c);
        if (ph > max_phi)
            max_phi = ph;
    }

    return max_phi;
}
