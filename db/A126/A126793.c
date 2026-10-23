/*
 * A126793  a(1) = 1; a(n+1) = Sum_{k|n} floor(a(k)/a(n/k)).
 * https://oeis.org/A126793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 162, u32 n <= 1130, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n ≥ 1, a(n+1) = Σ_{k|n} ⌊ a(k) / a(n/k) ⌋ */
#include <stdlib.h>

static seq_t *cache = NULL;         /* sequence cache */
static idx_t cache_cap = 0;         /* allocated capacity */
static idx_t computed = 0;          /* largest n for which a(n) is known */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = n + 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = p;
    cache_cap = new_cap;
}

/* Compute all values up to `upto` using the recurrence.
   Assumes cache has enough capacity. */
static void grow_to(idx_t upto)
{
    if (computed >= upto) return;

    /* Initialisation when nothing computed yet */
    if (computed == 0) {
        ensure_cache(1);
        cache[1] = (seq_t)1;
        computed = 1;
    }

    for (idx_t i = computed + 1; i <= upto; ++i) {
        /* i == 1 case is already handled above */
        idx_t m = i - 1;
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t other = m / d;
                sum += cache[d] / cache[other];
                if (d != other) sum += cache[other] / cache[d];
            }
        }
        cache[i] = sum;
    }
    computed = upto;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* invalid index, return 0 */
    ensure_cache(n);
    grow_to(n);
    return cache[n];
}
