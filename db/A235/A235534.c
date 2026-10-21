/*
 * A235534  a(n) = binomial(6*n, 2*n) / (4*n + 1).
 * https://oeis.org/A235534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, 2*n) / (4*n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for 64‑bit seq_t):                     */
/*  a(n) = [9*(6n-5)*(6n-1)*(3n-1)*(3n-2) /  (2n*(4n+1)*(2n-1)*(4n-1))] * a(n-1) */
/* (a(0) = 1)                                              */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "../utils/binomial.h"   /* for binomial(idx_t, idx_t) */

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* allocate / grow the cache up to n */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out‑of‑memory */
    cache = new_cache;
    cache_size = new_cap;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* initialise cache for the first two terms if needed */
    if (cache_size == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    } else if (cache_size <= 1) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    if (n < cache_size) return cache[n];

    /* If seq_t is 64‑bit we use the exact recurrence with 128‑bit intermediates */
    if (sizeof(seq_t) <= 8) {
        seq_t prev = cache[cache_size - 1];
        for (idx_t i = cache_size; i <= n; ++i) {
            /* compute a(i) from a(i-1) */
            __int128 num = (__int128)prev;
            num *= 9;
            num *= (6*i - 5);
            num *= (6*i - 1);
            num *= (3*i - 1);
            num *= (3*i - 2);

            __int128 den = (__int128)(2*i);
            den *= (4*i + 1);
            den *= (2*i - 1);
            den *= (4*i - 1);

            seq_t next = (seq_t)(num / den);   /* exact division */
            ensure_cache(i);
            cache[i] = next;
            prev = next;
        }
    } else {
        /* arbitrary‑precision build: use the definition directly */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t bin = binomial(6*i, 2*i);
            seq_t next = bin / ((seq_t)(4*i + 1));
            ensure_cache(i);
            cache[i] = next;
        }
    }
    return cache[n];
}
