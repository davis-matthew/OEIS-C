/*
 * A037693  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0,3.
 * https://oeis.org/A037693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037693
   Recurrence:  a(1) = 1
                a(n) = 9 * a(n-1) + d_n
   where d_n = {1,2,0,3}[(n-1) mod 4] (the periodic sequence 1,2,0,3).
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* Memoisation storage */
static seq_t *cache = NULL;   /* array of computed values (1‑indexed in sense a(1) stored at cache[0]) */
static idx_t cache_len = 0;   /* number of values currently stored (max index + 1) */
static idx_t cache_cap = 0;   /* allocated capacity of the array */

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;           /* already large enough */

    size_t new_cap = (size_t)n;                      /* new capacity requested (n values) */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();           /* allocation failure – terminate */

    /* copy existing values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free old array, if any */
    if (cache != NULL)
        free(cache);

    cache = new_cache;
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 – return 0 */

    /* make sure cache can hold up to n */
    ensure_cache(n);

    /* compute any missing values up to n (indices 0 … n-1) */
    const unsigned char pattern[4] = {1,2,0,3};

    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = (i == 0) ? (seq_t)0 : cache[i-1];
        unsigned digit = pattern[(unsigned)(i % 4)];   /* d_{i+1} */
        cache[i] = prev * (seq_t)9 + (seq_t)digit;
    }

    cache_len = n;   /* now all values up to n are stored */
    return cache[n-1];
}
