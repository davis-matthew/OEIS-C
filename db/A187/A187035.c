/*
 * A187035  Diagonal sums of number triangle A187034.
 * https://oeis.org/A187035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A187035
   Formula: a(n) = a(n-2) - a(n-3) + a(n-5)
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=-1, a(4)=1
   Indexing: a(0) is the first term.
   The function is memoised so that calls can be in any order. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure that cache contains values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return; /* already have it */

    /* grow the underlying array if necessary */
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;

    /* compute missing terms */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        seq_t val;
        if (k == 0)          val = (seq_t)1;
        else if (k == 1)     val = (seq_t)0;
        else if (k == 2)     val = (seq_t)1;
        else if (k == 3)     val = (seq_t)-1;
        else if (k == 4)     val = (seq_t)1;
        else                 val = cache[k-2] - cache[k-3] + cache[k-5];
        cache[k] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
