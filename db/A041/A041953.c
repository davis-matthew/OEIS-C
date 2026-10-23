/*
 * A041953  Denominators of continued fraction convergents to sqrt(499).
 * https://oeis.org/A041953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041953
 *
 * Denominators of continued fraction convergents to sqrt(499).
 *
 * Initial values (indices 0..11):
 *   1, 2, 3, 65, 68, 201, 8912, 18025, 26937,
 *   583702, 610639, 1804980
 * Recurrence for n > 11:
 *   a(n) = 8980*a(n-6) - a(n-12)
 *
 * The function computes terms on demand and memoises them.
 * It works for all supported seq_t types, using the builtin
 * integer operators when available and the class operators
 * when seq_t is an arbitrary‑precision integer class.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static idx_t  cache_alloc = 0;   /* allocated size of cache */
static idx_t  cache_computed = 0;/* number of terms actually computed */

/* First twelve terms of the sequence. */
static const seq_t init[12] = {
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)65, (seq_t)68, (seq_t)201,
    (seq_t)8912, (seq_t)18025, (seq_t)26937, (seq_t)583702,
    (seq_t)610639, (seq_t)1804980
};

/* Ensure that cache has space for at least n+1 terms. */
static void need_capacity(idx_t n)
{
    if (cache_alloc > n) return;        /* already enough */

    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();                  /* OOM – safe exit */

    /* If cache was uninitialised, cache is now new_sz zeroed by realloc. */
    cache = tmp;
    cache_alloc = new_sz;
}

/* Compute all terms up to index up_to, inclusive.
 * The function uses memoisation: previously computed terms are reused.
 */
static void compute_upto(idx_t up_to)
{
    /* Initialise the first twelve terms if we have not yet started. */
    if (cache_computed == 0) {
        /* Determine how many of the first twelve terms we need. */
        idx_t limit = (up_to < 11) ? up_to : 11;
        for (idx_t i = 0; i <= limit; ++i)
            cache[i] = init[i];
        cache_computed = limit + 1;
    }

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = cache_computed; i <= up_to; ++i) {
        seq_t term;
        if (i < 12) {
            term = init[i];           /* already stored, but harmless */
        } else {
            /* term = 8980 * cache[i-6] - cache[i-12] */
            term = cache[i-6];
            term = term * (seq_t)8980;   /* use host type multiplication */
            term = term - cache[i-12];
        }
        cache[i] = term;
    }
    cache_computed = up_to + 1;
}

/* Return a(n) – 0‑based index. */
seq_t a(idx_t n)
{
    need_capacity(n);       /* guarantee room for n+1 terms */
    compute_upto(n);        /* compute up to required index */
    return cache[n];
}
