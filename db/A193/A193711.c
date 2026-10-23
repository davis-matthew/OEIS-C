/*
 * A193711  Partial sums of the union of squares and triangular numbers.
 * https://oeis.org/A193711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A005214(k) – partial sums of the union of
 *            squares (k²) and triangular (*k*(k+1)/2) numbers
 * Range:     u8 n <= 13, u16 n <= 100, u32 n <= 4212, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Internal cache for cumulative sums */
static seq_t *cache    = NULL;   /* array of sums */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len  = 0;     /* number of sums stored */

/* Next values of the two sequences */
static idx_t sq_k   = 1;         /* next square index   */
static idx_t tri_m  = 1;         /* next triangular index */
static seq_t sq_val = 1;         /* current square   */
static seq_t tri_val= 1;         /* current triangular */

/* Ensure the cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)                /* already ready */
        return;

    /* Grow capacity to at least n */
    size_t needed = n;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < needed)
        cache_cap <<= 1;

    /* Allocate / reallocate storage */
    if (cache) {
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate new terms and accumulate sums */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t next;
        /* Pick the smaller candidate, handling duplicates */
        if (sq_val <= tri_val) {
            next = sq_val;
            sq_k++;
            sq_val = (seq_t)sq_k * sq_k;          /* update square */
            if (next == tri_val) {                 /* same value occurred once */
                tri_m++;
                tri_val = (seq_t)tri_m * (tri_m + 1) / 2;
            }
        } else {
            next = tri_val;
            tri_m++;
            tri_val = (seq_t)tri_m * (tri_m + 1) / 2;
        }

        size_t idx = (size_t)i;
        if (i == 0)
            cache[idx] = next;
        else
            cache[idx] = cache[idx - 1] + next;
    }

    cache_len = n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
