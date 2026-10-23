/*
 * A138996  First differences of Frobenius numbers for 5 successive numbers
 *          A138985.
 * https://oeis.org/A138996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138996: First differences of Frobenius numbers for 5 successive numbers
   Recurrence: a(n) = 2*a(n-4) - a(n-8)  for n > 8
   Initial values: a(1)=1, a(2)=1, a(3)=1, a(4)=7,
                   a(5)=2, a(6)=2, a(7)=2, a(8)=12
   The function is memoised; calls with arbitrary n up to ~10 000
   run in O(n) time with negligible constant factors.
*/
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cached values, 1‑based (cache[0] unused) */
static size_t cache_cap = 0;     /* allocated capacity (in elements) */
static idx_t  cache_len  = 0;    /* first unused index (next value to compute) */

/* initialise the cache with the first eight terms */
static void init_cache(void)
{
    if (cache != NULL) return;

    /* start with a moderate capacity */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    /* zero‑index unused */
    cache[0] = 0;
    /* initial values */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)7;
    cache[5] = (seq_t)2;
    cache[6] = (seq_t)2;
    cache[7] = (seq_t)2;
    cache[8] = (seq_t)12;

    cache_len = 9;          /* next index that needs computing */
}

/* ensure the cache can hold up to index n (inclusive) */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap;
    while ((size_t)n >= new_cap) new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out-of‑range indices are undefined */

    init_cache();
    if ((size_t)n >= cache_cap) grow_cache(n);

    /* compute missing values up to n */
    while (cache_len <= n) {
        seq_t val;
        if (cache_len <= 8) {
            /* initial seed values already present */
            val = cache[cache_len];
        } else {
            /* recurrence: a(k) = 2*a(k-4) - a(k-8) */
            seq_t t1 = cache[cache_len - 4];
            val = t1 + t1;          /* 2 * a(k-4) */
            val -= cache[cache_len - 8]; /* minus a(k-8) */
        }
        cache[cache_len] = val;
        ++cache_len;
    }
    return cache[n];
}
