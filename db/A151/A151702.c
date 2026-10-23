/*
 * A151702  a(0)=1, a(1)=0; a(2^i + j) = a(j) + a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; a(2^i + j)=a(j)+a(j+1) for 0<=j<2^i
 * Range:     u8 n <= 2039, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation storage */
static idx_t cache_cap = 0;
static seq_t *cache = NULL;
static unsigned char *done = NULL;

/* Make the cache large enough for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL || n >= cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= n)
            new_cap <<= 1;

        seq_t *new_cache   = (seq_t*)realloc(cache,  new_cap * sizeof(seq_t));
        unsigned char *new_done = (unsigned char*)realloc(done,  new_cap * sizeof(unsigned char));
        cache = new_cache;
        done  = new_done;

        /* initialise the newly allocated entries */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            done[i] = 0;

        cache_cap = new_cap;
    }
}

/* Recursive evaluation with memoisation. */
static seq_t rec_value(idx_t n)
{
    if (done[n])
        return cache[n];

    seq_t val;
    if (n == 0)
        val = (seq_t)1;
    else if (n == 1)
        val = (seq_t)0;
    else
    {
        /* largest power of two <= n */
        idx_t pow2 = 1;
        while (pow2 <= n >> 1)
            pow2 <<= 1;

        idx_t j = n - pow2;
        val = rec_value(j) + rec_value(j + 1);
    }

    cache[n] = val;
    done[n]  = 1;
    return val;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return rec_value(n);
}
