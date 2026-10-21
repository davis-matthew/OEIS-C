/*
 * A061471  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 2.
 * https://oeis.org/A061471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose alternating digit sum
 *            (first digit minus second plus third minus …) equals 2
 * Range:     u8 n <= 24, u16 n <= 4110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine keeps a cache of all terms found so far; subsequent
   calls merely look up the cache or extend it until the requested
   index is available.  It is O(number of tested integers) overall. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alt_digit_sum.h"

/* Cache state ------------------------------------------------------- */
static seq_t *cache          = NULL;   /* array of a(i)  */
static size_t cache_cap      = 0;      /* allocated capacity */
static size_t cache_len      = 0;      /* number of terms stored */
static uint64_t next_candidate = 1;    /* candidate to test next */

/* Ensure the cache can hold at least n terms. */
static void ensure_cache_cap(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);       /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used (OEIS offset is 1) */

    /* If already computed, return it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Extend cache until we have the n‑th term. */
    ensure_cache_cap((size_t)n);

    while (cache_len < (size_t)n) {
        uint64_t cur = next_candidate++;
        if (alt_digit_sum((seq_t)cur) == 2) {
            cache[cache_len++] = (seq_t)cur;
        }
    }
    return cache[n - 1];
}
