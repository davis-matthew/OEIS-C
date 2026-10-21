/*
 * A078022  Expansion of (1-x)/(1-x+2*x^2+2*x^3).
 * https://oeis.org/A078022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0, a(2)=-2, a(n)=a(n-1)-2*a(n-2)-2*a(n-3) for n≥3
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 39, i64 n <= 79, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1‑x)/(1‑x+2*x²+2*x³) */

#include <stddef.h>
#include <stdlib.h>

/* cache for already computed terms */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;

/* grow the cache up to index n (inclusive) */
static void seq_grow_to(idx_t n)
{
    /* nothing to do if we already hold this index */
    if (seq_cache_len > (size_t)n)
        return;

    size_t need = (size_t)n + 1;
    if (seq_cache_cap < need) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = seq_cache_len; i < need; ++i) {
        if (i == 0)
            seq_cache[i] = (seq_t)1;
        else if (i == 1)
            seq_cache[i] = (seq_t)0;
        else if (i == 2)
            seq_cache[i] = (seq_t)-2;
        else
            seq_cache[i] = seq_cache[i-1]
                           - (seq_t)2 * seq_cache[i-2]
                           - (seq_t)2 * seq_cache[i-3];
    }
    seq_cache_len = need;
}

seq_t a(idx_t n)
{
    seq_grow_to(n);
    return seq_cache[(size_t)n];
}
