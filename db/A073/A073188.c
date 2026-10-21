/*
 * A073188  n appears 1+[n/3] times.
 * https://oeis.org/A073188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(6*n + 6) - 3/2)
 * Range:     u8 n <= 6900, u16 n <= 6900, u32 n <= 6900, u64 n <= 6900,
 *            u128 n <= 6900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0) = 0; for n ≥ 1
 * a(n) = 1 + a(n - 1 - floor(a(n - 1)/3)) */
#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamic cache of computed values   */
static idx_t cache_cap = 0;       /* current number of cached entries  */

/* Ensure that cache holds values up to index n (inclusive).   */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;          /* already computed */

    /* enlarge cache */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;

    /* compute fresh entries */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {                   /* base value */
            cache[0] = (seq_t)0;
        } else {
            seq_t prev = cache[i - 1];
            seq_t sub  = prev / 3;            /* floor(a(i-1)/3) */
            idx_t  idxPrev = i - 1 - (idx_t)sub;
            cache[i] = (seq_t)1 + cache[idxPrev];
        }
    }
    cache_cap = new_cap;
}

/* Public entry point for OEIS sequence A073188 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
