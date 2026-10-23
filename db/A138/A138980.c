/*
 * A138980  a(0)=1; for n>=1, a(n) = smallest prime > a(n-1) such that
 *          (sum{k=0 to n} a(k)) is composite.
 * https://oeis.org/A138980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; for n≥1, a(n) is the smallest prime > a(n‑1) such that
   S(n)=∑_{k=0}^n a(k) is composite.  The sequence is built lazily and
   cached so calls can be made in any order.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t)   */

static seq_t *cache = NULL;       /* cache of a(k) values */
static size_t cache_cap = 0;      /* allocated capacity (in elements) */
static size_t cache_len = 0;      /* number of stored elements (last idx+1) */
static seq_t sum_accum = 0;       /* cumulative sum of a(0)..a(last) */

/* Ensure the cache can store indices up to `anew` (inclusive).  */
static void
ensure_cache_capacity(idx_t anew)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        sum_accum = (seq_t)1;
        cache_len = 1;
    }
    if ((size_t)anew + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)anew + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Extend the sequence up to index `n` (inclusive).  */
static void
extend_to(idx_t n)
{
    if (cache == NULL)
        return;   /* should not happen: ensure_cache_capacity guarantees it */

    size_t last_idx = cache_len - 1;
    if ((idx_t)last_idx >= n)
        return;   /* already computed */

    for (idx_t i = last_idx + 1; i <= n; ++i) {
        seq_t candidate = cache[i - 1] + (seq_t)1;     /* start just above a(i‑1) */
        if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
            candidate += (seq_t)1;                    /* skip even numbers >2 */

        for (;;) {
            /* Skip non‑primes.  */
            if (!is_prime_big(candidate)) {
                candidate += (seq_t)1;
                if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
                    candidate += (seq_t)1;
                continue;
            }

            seq_t new_sum = sum_accum + candidate;
            if (is_prime_big(new_sum)) {             /* sum must be composite */
                candidate += (seq_t)1;
                if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
                    candidate += (seq_t)1;
                continue;
            }

            /* Found the required prime.  */
            cache[i] = candidate;
            sum_accum = new_sum;
            break;
        }
    }
    cache_len = (size_t)n + 1;
}

/* Return a(n) with static caching.  */
seq_t
a(idx_t n)
{
    ensure_cache_capacity(n);
    extend_to(n);
    return cache[(size_t)n];
}
