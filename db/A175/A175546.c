/*
 * A175546  Primes p such that sod(p)=2*sod(nextprime(p)).
 * https://oeis.org/A175546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime p such that digit_sum(p) =
 *            2*digit_sum(next_prime(p))
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_sum, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists primes satisfying sod(p)=2*sod(nextprime(p)). */

/* Use provided utilities for digit sums and prime searching. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/next_prime.h"

/* Static cache for the sequence values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of stored terms */
static idx_t cache_cap = 0;   /* allocated capacity */
static uint64_t last_checked = 0; /* last prime checked */

/* Ensure the cache can hold at least 'needed' elements. */
static void
ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but harmless */

    ensure_cache_capacity(n);

    while (cache_len < n) {
        /* Find the next prime after the last one we examined. */
        uint64_t p = next_prime(last_checked + 1);
        uint64_t q = next_prime(p);

        seq_t sod_p = digit_sum((seq_t)p);
        seq_t sod_q = digit_sum((seq_t)q);

        if (sod_p == (sod_q << 1u))
            cache[cache_len++] = (seq_t)p;      /* store qualifying prime */

        last_checked = p;                        /* advance search frontier */
    }

    return cache[n - 1];
}
