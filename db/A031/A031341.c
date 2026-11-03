/*
 * A031341  a(n) = prime(8*n).
 * https://oeis.org/A031341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(8*n) – the 8*n‑th prime
 * Range:     u8 n <= 6, u16 n <= 817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 8749 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* static cache of generated primes */
static uint64_t *prime_cache = NULL;
static size_t     prime_cache_cnt = 0;

/* ensure that at least 'nb' primes are in prime_cache (nb >= 1) */
static void
ensure_primes(idx_t nb)
{
    if (prime_cache_cnt >= nb)
        return;                       /* already have enough */

    /* compute an upper bound for the nb‑th prime */
    idx_t limit = _prime_limit_for_idx(nb);

    uint64_t *new_cache = NULL;
    size_t    new_cnt   = 0;

    build_primes_upto(limit, &new_cache, &new_cnt);

    /* new_cnt should be >= nb, but in the unlikely event it isn't,
       increase the limit and try again. */
    while (new_cnt < nb)
    {
        /* increase limit by 50% */
        limit = limit + limit / 2;
        free(new_cache);
        build_primes_upto(limit, &new_cache, &new_cnt);
    }

    /* replace the old cache */
    free(prime_cache);
    prime_cache       = new_cache;
    prime_cache_cnt   = new_cnt;
}

/* sequence a(n) = prime(8*n),  n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)                 /* behavior below offset is irrelevant */
        return (seq_t)0;

    idx_t needed = n * 8;               /* 8*n-th prime index */
    ensure_primes(needed);

    uint64_t result = prime_cache[needed - 1];
    return (seq_t)result;
}
