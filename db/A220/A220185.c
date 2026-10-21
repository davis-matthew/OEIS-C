/*
 * A220185  Numbers n such that n^2 + n(n+1) is an oblong number (A002378).
 * https://oeis.org/A220185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A220185
 * 
 * The terms satisfy the recurrence
 *     a(1)=0, a(2)=10, a(3)=348
 *     a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3)   (n>3)
 * The closed form is provided in the OEIS entry but this
 * implementation uses the recurrence and memoises computed
 * values for fast repeated calls.
 */

#include <stddef.h>
#include <stdlib.h>

/* Utility to grow a dynamically allocated array to the required size.
 * The array element type is seq_t and the size is tracked by *size. */
static void grow_array(seq_t **arr, size_t *size, size_t newsize)
{
    if (newsize <= *size)
        return;
    seq_t *tmp = (seq_t *)realloc(*arr, newsize * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed */
    *arr = tmp;
    *size = newsize;
}

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)                    /* behaviour for n<1 is irrelevant */
        return (seq_t)0;

    size_t idx = (size_t)n;       /* 1‑based index used in the cache    */

    /* initialise cache with first three values once */
    if (cache_len == 0) {
        grow_array(&cache, &cache_len, 3);
        cache[0] = (seq_t)0;      /* a(1) */
        cache[1] = (seq_t)10;     /* a(2) */
        cache[2] = (seq_t)348;    /* a(3) */
        cache_len = 3;
    }

    /* if the requested value is already cached, return it */
    if (idx <= cache_len)
        return cache[idx - 1];

    /* extend cache to hold the requested index */
    size_t old_len = cache_len;
    grow_array(&cache, &cache_len, idx);   /* cache_len now equals idx */

    /* compute missing terms using the recurrence */
    for (size_t i = old_len; i < idx; ++i) {
        /* i corresponds to term a(i+1)   (0‑based array) */
        seq_t term = 35 * cache[i - 1];
        term -= 35 * cache[i - 2];
        term += cache[i - 3];
        cache[i] = term;
    }

    return cache[idx - 1];
}
