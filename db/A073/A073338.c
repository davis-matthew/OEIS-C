/*
 * A073338  Positive values of n for which 4n^2-10n+7 is prime.
 * https://oeis.org/A073338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 5087, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073338: n such that 4*n^2-10*n+7 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* Static cache for memoization of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored in cache */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t next_k = 1;       /* next integer to test */

/* Grow the cache capacity using seq_realloc. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();                /* unrecoverable memory error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of the OEIS sequence A073338. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, guard */

    while (cache_len < (size_t)n) {
        seq_t kk = (seq_t)next_k;
        seq_t val = 4 * kk * kk - 10 * kk + 7;
        if (isPrimeGeneric(val)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = kk;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
