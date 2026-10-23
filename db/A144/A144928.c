/*
 * A144928  Values of k arising in A144927.
 * https://oeis.org/A144928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*a(n-1) - a(n-2) with a(1) = 49 , a(2) = 5341
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence grows rapidly; the recurrence is used to generate any term. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* dynamic array storing a(1) .. a(cache_size) */
static idx_t  cache_size = 0;     /* number of terms stored   */
static idx_t  cache_cap   = 0;     /* array capacity          */

/* ensure the cache can hold at least n terms, filling values as needed */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* initialise */
    if (cache_cap == 0) {
        cache_cap = 8;                          /* initial arbitrary capacity */
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* guard against allocation failure – undefined behaviour if fails */
        if (seq_cache == NULL) abort();

        /* store a(1) and a(2) */
        if (cache_cap < 2) {
            cache_cap = 2;
            seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
        }
        seq_cache[0] = (seq_t)49;
        seq_cache[1] = (seq_t)5341;
        cache_size = 2;
    }

    /* extend cache to n */
    for (; cache_size < n; ++cache_size) {
        /* grow storage if necessary */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
            if (seq_cache == NULL) abort();
        }
        /* recurrence: a(k+1) = 110*a(k) - a(k-1)  (here k = cache_size-1) */
        seq_t next = (seq_t)110 * seq_cache[cache_size - 1] - seq_cache[cache_size - 2];
        seq_cache[cache_size] = next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – undefined for n=0 */
    ensure_cache(n);
    return seq_cache[n - 1];
}
