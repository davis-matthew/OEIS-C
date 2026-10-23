/*
 * A115129  Partial sums of A005587. Fourth column of triangle A115127.
 * https://oeis.org/A115129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Aleph OEIS A115129 – partial sums of A005587 (fourth column of A115127)
   Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
   Initial values: a(0)=14, a(1)=56, a(2)=146, a(3)=311, a(4)=586, a(5)=1015
*/

#include <stdlib.h>

/* static cache for memoisation (indexed from 0) */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of elements currently stored      */
static size_t cache_cap  = 0;   /* allocated capacity of the cache          */

/* initialise the cache with the first 6 terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;   /* already initialised */

    cache_cap = 6;
    /* allocation that works for both C and C++ */
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        return;   /* allocation failure – unlikely in test harness */

    cache[0] = (seq_t)14;
    cache[1] = (seq_t)56;
    cache[2] = (seq_t)146;
    cache[3] = (seq_t)311;
    cache[4] = (seq_t)586;
    cache[5] = (seq_t)1015;

    cache_len = 6;
}

/* grow capacity to at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return;   /* allocation failure – unlikely */

    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*                     OEIS A115129                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();
    size_t i = (size_t)n;

    /* already computed? */
    if (i < cache_len)
        return cache[i];

    /* need to grow cache */
    ensure_capacity(i + 1);

    /* compute all missing terms up to i */
    for (size_t k = cache_len; k <= i; ++k)
    {
        /* recurrence: a(k) = 6*a(k-1) - 15*a(k-2) + 20*a(k-3)
                           - 15*a(k-4) + 6*a(k-5) - a(k-6) */
        seq_t t0 = cache[k - 1];
        seq_t t1 = cache[k - 2];
        seq_t t2 = cache[k - 3];
        seq_t t3 = cache[k - 4];
        seq_t t4 = cache[k - 5];
        seq_t t5 = cache[k - 6];

        seq_t val = 6 * t0;          /* 6*a(k-1)   */
        val -= 15 * t1;              /* -15*a(k-2) */
        val += 20 * t2;              /* +20*a(k-3) */
        val -= 15 * t3;              /* -15*a(k-4) */
        val += 6 * t4;               /* +6*a(k-5)  */
        val -=     t5;               /* -a(k-6)    */

        cache[k] = val;
    }

    cache_len = i + 1;
    return cache[i];
}
