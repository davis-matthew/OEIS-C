/*
 * A039123  Numbers whose base-10 representation has the same number of 1's
 *          and 5's.
 * https://oeis.org/A039123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose decimal *
 *            representation contains the same number of 1's and 5's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"   /* provides digit_counts(seq_t x, unsigned char cnt[10]) */

static seq_t *cache = NULL;          /* array of found numbers */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* capacity of the array */
static idx_t next_candidate = 0;     /* the smallest integer to test next */

/* Grow the cache array if needed.  Uses realloc as required by the
 * C11/C++17 rules.  The caller is responsible for freeing `cache`
 * at program exit if desired (not necessary in the online judge
 * environment). */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();  /* unrecoverable allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Append a new value to the cache, growing the array if necessary. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Main sequence function.  It lazily computes terms on demand,
 * storing them so that subsequent calls with larger n are fast.
 * The function works for any build (uint8_t, …, uint64_t,
 * unsigned __int128, or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Validate that n >= 1; behaviour for n < 1 is undefined per spec. */
    if (n == 0) return 0;   /* not used, but keep return value defined */

    /* Generate terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        unsigned char cnt[10] = {0};
        digit_counts(val, cnt);
        if (cnt[1] == cnt[5]) {
            cache_append(val);
        }
        ++next_candidate;
    }
    return cache[n - 1];  /* zero‑based indexing into cache */
}
