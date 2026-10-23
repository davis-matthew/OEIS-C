/*
 * A100138  a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 2^(n-5k).
 * https://oeis.org/A100138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 2^(n-5k) Recurrence:
 *            a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) + 2*a(n-6) Initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamically growing array of computed terms */
static size_t cache_size = 0;   /* number of elements allocated in cache */

/* Ensure that cache contains values up to index n, computing missing terms via the
   recurrence.  This function is called by a() before returning the requested value. */
static void ensure_capacity(idx_t n)
{
    /* If we already have the requested entry, do nothing */
    if ((size_t)n < cache_size)
        return;

    /* Determine new allocation size: at least n+1, but grow a bit to reduce reallocs */
    size_t newsize = (size_t)n + 1;
    if (newsize < 16)
        newsize = 16;           

    /* Reallocate the cache */
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!cache) {
        /* Allocation failed – aborting as per typical kernel behaviour */
        abort();
    }

    /* Compute and store the new terms */
    for (size_t i = cache_size; i < newsize; ++i) {
        switch (i) {
            case 0: cache[i] = 1; break;
            case 1: cache[i] = 2; break;
            case 2: cache[i] = 4; break;
            case 3: cache[i] = 8; break;
            case 4: cache[i] = 16; break;
            case 5: cache[i] = 32; break;
            default: {
                /* Use the recurrence to compute a(i) */
                seq_t term = cache[i-1] * (seq_t)6;
                term += cache[i-3] * (seq_t)8;
                term += cache[i-6] * (seq_t)2;
                term -= cache[i-2] * (seq_t)12;
                cache[i] = term;
            }
        }
    }

    cache_size = newsize;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return cache[(size_t)n];
}
