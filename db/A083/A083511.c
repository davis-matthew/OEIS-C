/*
 * A083511  Members of A000124 which are multiples of 11.
 * https://oeis.org/A083511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083511 – multiples of 11 in the central polygonal numbers.
   Central polygonal numbers are defined by
       p(k) = k*(k+1)/2 + 1.
   This function returns the n‑th element of the subsequence of those
   p(k) which are divisible by 11.  The terms are generated on demand
   and cached so that repeated calls (in any order) are fast. */
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;      /* array of stored terms   */
    static size_t cache_len = 0;     /* number of terms stored  */
    static size_t cache_cap = 0;     /* current capacity        */
    static idx_t next_k = 0;         /* next k value to test    */

    /* Return cached value if possible */
    if (n < cache_len)
        return cache[n];

    /* Grow cache until it contains the requested index */
    for ( /* nothing */; cache_len <= n; /* nothing */) {
        /* Ensure capacity for one more element */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < cache_len + 1)
                new_cap = cache_len + 1;
            seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
            if (!new_arr) abort();  /* allocation failure */
            cache = new_arr;
            cache_cap = new_cap;
        }

        /* Compute p(next_k) */
        seq_t val = (seq_t)next_k * ((seq_t)next_k + 1) / 2 + 1;
        if (val % 11 == 0) {
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    return cache[n];
}
