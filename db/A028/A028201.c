/*
 * A028201  Expansion of 1/((1-6x)(1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A028201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 80*a(n-2) + 7^(n+1) - 6^(n+1); initial
 *            values: a(0)=1, a(1)=31 The implementation uses memoisation with
 *            a dynamic array
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *cache = NULL;    /* cache[n] = a(n) */
static size_t cache_cap = 0;   /* capacity of cache array */
static size_t cache_sz = 0;    /* number of terms currently stored */
static seq_t p7 = 0;           /* holds 7^(current_index) before multiplication */
static seq_t p6 = 0;           /* holds 6^(current_index) before multiplication */

/* initialise the cache on first use */
static void init_cache(void)
{
    if (cache == NULL) {
        /* start with a modest capacity */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* in a production environment you would handle allocation failure
               here. For the purposes of the OEIS evaluation harness it is
               assumed the allocation succeeds. */
            abort();
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache_sz = 2;

        /* For computing a(2) we need 7^3 and 6^3.  We keep p7 and p6
           as 7^(cache_sz) and 6^(cache_sz) before multiplying in the loop. */
        p7 = (seq_t)49;   /* 7^2 */
        p6 = (seq_t)36;   /* 6^2 */
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_cache();

    /* If the requested term is already cached, return it immediately. */
    if ((size_t)n < cache_sz)
        return cache[(size_t)n];

    /* Extend the cache until it contains a(n). */
    while (cache_sz <= (size_t)n) {
        /* Grow the cache if necessary. */
        if (cache_sz == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (cache == NULL) {
                abort();
            }
        }

        /* Update the powers for the next term: multiply by 7 and 6. */
        p7 = p7 * (seq_t)7;
        p6 = p6 * (seq_t)6;

        /* Compute a(cache_sz) using the recurrence
           a(n) = 18*a(n-1) - 80*a(n-2) + 7^(n+1) - 6^(n+1). */
        seq_t term = (seq_t)18 * cache[cache_sz - 1]
                   - (seq_t)80 * cache[cache_sz - 2]
                   + p7
                   - p6;

        /* Store the result. */
        cache[cache_sz] = term;
        cache_sz++;
    }

    return cache[(size_t)n];
}
