/*
 * A137471  Positive numbers n such that n^3 +- (n+1) are primes.
 * https://oeis.org/A137471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137471: positive integers n such that n^3 ± (n+1) are primes. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Cache to store previously found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 2;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, return 0 as a fallback */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        unsigned __int128 v   = next_candidate;
        unsigned __int128 cube = v * v * v;            /* v³ */
        unsigned __int128 plus  = cube + v + 1;        /* v³ + (v+1) */
        unsigned __int128 minus = cube - (v + 1);      /* v³ – (v+1) */

        seq_t pplus  = (seq_t)plus;
        seq_t pminus = (seq_t)minus;

        if (isPrimeGeneric(pplus) && isPrimeGeneric(pminus)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
