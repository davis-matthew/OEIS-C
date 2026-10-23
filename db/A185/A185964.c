/*
 * A185964  Diagonal sums of number triangle A185962.
 * https://oeis.org/A185964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} A185962(n-k,k) Equivalent linear
 *            recurrence for n ≥ 4: a(n) = a(n‑1) – 2·a(n‑3) + a(n‑4) with
 *            initial values a(0)=1, a(1)=‑1, a(2)=0, a(3)=‑2
 * Range:     i8 n <= 22, i16 n <= 47, i32 n <= 93, i64 n <= 183,
 *            i128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS reference: A185964 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;   /* memoized values */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* Ensure that cache[n] exists for all n up to the requested value */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;          /* already computed */

    size_t need = (size_t)n + 1;
    if (need > cache_cap) {                     /* grow capacity */
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    const seq_t two = (seq_t)2;
    for (size_t i = cache_size; i < need; ++i) {
        if (i == 0)      cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)-1;
        else if (i == 2) cache[i] = (seq_t)0;
        else if (i == 3) cache[i] = (seq_t)-2;
        else             cache[i] = cache[i-1] - two*cache[i-3] + cache[i-4];
    }

    cache_size = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
