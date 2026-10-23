/*
 * A130149  a(0) = a(1) = 1. For n >= 1, (a(n)+a(n-1)) is repeated a(n) times.
 * https://oeis.org/A130149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130149
   Formula: a(0)=a(1)=1; for n≥1, a(n+i)=a(n)+a(n-1) for 1≤i≤a(n). */

#include <stdlib.h>

/* Static cache that is filled the first time the function is called. */
static seq_t *cache      = NULL;     /* array holding a(0)..a(k) */
static size_t cache_cap  = 0;        /* allocated capacity of cache   */
static idx_t  cache_len  = 0;        /* number of elements currently stored */
static idx_t  proc_next  = 1;        /* next n to process (≥1)         */

seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 4;                          /* start with small capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
        proc_next = 1;
    }

    /* Generate terms up to index n. */
    while (cache_len <= n) {
        /* For safety in case something goes wrong. */
        if (proc_next >= cache_len) break;

        /* Compute the value to be repeated. */
        seq_t sum = cache[proc_next] + cache[proc_next - 1];

        /* Append sum 'cache[proc_next]' times. */
        idx_t i = 0;
        for (i = 0; i < cache[proc_next]; ++i) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }
        /* Move to the next n that needs processing. */
        ++proc_next;
    }

    return cache[n];
}
