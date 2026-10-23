/*
 * A030074  Squares which are palindromes in base 14.
 * https://oeis.org/A030074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th non‑negative square whose base‑14 representation
 *            * is a palindrome. The sequence starts with a(1)=0, a(2)=1,
 *            a(3)=4, a(4)=9, …
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 37, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_palindrome, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/is_palindrome.h"

/* Cache for the generated squares that are palindromes in base 14. */
static seq_t *cache      = NULL;     /* array of cached terms                */
static size_t cache_cap  = 0;        /* current capacity of the array        */
static size_t cache_len  = 0;        /* number of terms stored in the cache  */
static idx_t  next_k     = 0;        /* next integer whose square has not been tested */

/* Initialise the cache with a modest capacity.  The capacity grows automatically
 * if more terms are required. */
static void cache_init(void)
{
    if (cache != NULL)
        return;
    cache_cap = 20000;                     /* enough for n ≈ 10 000 */
    cache     = seq_alloc_new_array(cache_cap);
    cache_len = 0;
    next_k    = 0;
}

/* Ensure that the cache can hold at least 'needed' elements. */
static void cache_grow_if_needed(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Double the capacity until it is sufficient. */
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (cache_len > 0)
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined offset; return 0 to avoid UB */
        return 0;

    cache_init();

    /* Generate squares until we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_k * (seq_t)next_k;
        if (is_palindrome(val, (idx_t)14))
            cache_grow_if_needed(cache_len + 1);   /* grow if necessary */

        if (is_palindrome(val, (idx_t)14))
            cache[cache_len++] = val;

        ++next_k;
    }

    return cache[n - 1];
}
