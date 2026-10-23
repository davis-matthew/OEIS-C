/*
 * A189974  Numbers m such that d(m-1) = d(m+1) = 4, where d(k) is the number
 *          of divisors of k (A000005).
 * https://oeis.org/A189974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4232, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search for numbers m with d(m-1) = d(m+1) = 4.
   Memoize results.  Complexity O(N * sqrt(m)) for the first N terms. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ensure_capacity.h"

/* Return true iff n has exactly four positive divisors.  n must be > 1. */
static bool has_four_divisors(uint64_t n)
{
    if (n <= 1) return false;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (exps[i] + 1);
        if (prod > 4) return false;
    }
    return prod == 4;
}

/* Static cache for the sequence. */
static seq_t *cache_vals = NULL;   /* array of stored terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */
static uint64_t last_checked = 1;  /* last m examined */

/* Return the n-th term of A189974 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        ++last_checked;                     /* next candidate m */
        uint64_t m = last_checked;

        if (has_four_divisors(m - 1) && has_four_divisors(m + 1)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache_vals, sizeof(seq_t));
            }
            cache_vals[cache_len++] = (seq_t)m;
        }
    }
    return cache_vals[n - 1];               /* zero‑based indexing */
}
