/*
 * A081908  a(n) = 2^n*(n^2 - n + 8)/8.
 * https://oeis.org/A081908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n*(n^2 - n + 8)/8 * Recurrence (used for efficient
 *            computation): * a(0) = 1, a(1) = 2, a(2) = 5 * a(n) = 6*a(n-1) -
 *            12*a(n-2) + 8*a(n-3) for n >= 3 * * The recurrence is
 *            integer‑preserving for all n and avoids huge intermediate *
 *            powers of 2. A static cache is used so that multiple calls...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms */
static idx_t  cache_size = 0;     /* number of elements stored */

/* Ensure cache holds values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0) {
        /* First allocation: at least three elements for the seed values. */
        idx_t sz = (n >= 3) ? (n + 1) : 3;
        cache = (seq_t *)calloc((size_t)sz, sizeof(seq_t));
        cache_size = sz;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – abort. */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        if (cache_size < 3) {
            /* Initialize missing seed values if the cache was smaller. */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)5;
        }
        cache_size = new_size;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;  /* already computed */
        cache[i] = (seq_t)(6 * cache[i-1] - 12 * cache[i-2] + 8 * cache[i-3]);
    }
}

seq_t a(idx_t n)
{
    if (n < 3) {
        /* Handle base cases directly */
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)5;
            default: return (seq_t)0; /* unreachable */
        }
    }
    ensure_cache(n);
    return cache[n];
}
