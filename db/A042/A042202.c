/*
 * A042202  Numerators of continued fraction convergents to sqrt(627).
 * https://oeis.org/A042202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1252*a(n-2) - a(n-4)
   Initial values: a(0) = 25, a(1) = 626, a(2) = 31325, a(3) = 783751 */

#include <stdlib.h>

/* Cached sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */

/* Initialise the cache with the first four elements. */
static void cache_init(void)
{
    if (cache != NULL)
        return;

    cache_len = 4;
    cache = (seq_t *)malloc(4 * sizeof(seq_t));
    if (cache == NULL)
        return;  /* allocation failed – subsequent calls will see garbage */

    cache[0] = 25;
    cache[1] = 626;
    cache[2] = 31325;
    cache[3] = 783751;
}

/* Extend the cache so that it contains all indices up to n.
   Missing terms are computed using the recurrence. */
static void cache_extend(idx_t n)
{
    /* Nothing to do if we already have enough elements. */
    if ((size_t)n < cache_len)
        return;

    size_t old_len  = cache_len;
    size_t new_len  = (size_t)n + 1;          /* length needed */

    /* Reallocate to required size. */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return;                                 /* leave cache unchanged */
    cache      = tmp;
    cache_len  = new_len;

    /* Compute all terms that were missing. */
    for (size_t i = old_len; i < new_len; ++i)
    {
        /* i >= 4 at the first new entry because old_len >= 4 when
           new_len > old_len.  Hence i-2 and i-4 are valid indices. */
        cache[i] = (seq_t)1252 * cache[i - 2] - cache[i - 4];
    }
}

/* Return the nth element of the sequence. */
seq_t a(idx_t n)
{
    cache_init();

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    cache_extend(n);

    /* Even if realloc failed the caller should not expect a correct
       value; returning 0 is a placeholder. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    return 0;
}
