/*
 * A038407  Coordination sequence T2 for Zeolite Code AWO.
 * https://oeis.org/A038407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038407: Coordination sequence T2 for Zeolite Code AWO
 *
 * Recurrence (n > 21):
 *   a(n) = -a(n-2) + a(n-5) + 3*a(n-7) + 2*a(n-9)
 *          - 2*a(n-12) - 3*a(n-14) - a(n-16)
 *          + a(n-19) + a(n-21)
 * Initial terms:
 *   a(0)=1, a(1)=4, a(2)=10, a(3)=21, a(4)=36, a(5)=53, a(6)=76, a(7)=108,
 *   a(8)=142, a(9)=173, a(10)=210, a(11)=259, a(12)=310, a(13)=360,
 *   a(14)=418, a(15)=482, a(16)=546, a(17)=615, a(18)=692, a(19)=769,
 *   a(20)=848, a(21)=940
 */

#include <stdlib.h>               /* abort, size_t */
#include "../utils/seq_realloc.h"  /* seq_realloc */

static const seq_t init_vals[22] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)21, (seq_t)36,
    (seq_t)53, (seq_t)76, (seq_t)108, (seq_t)142, (seq_t)173,
    (seq_t)210,(seq_t)259,(seq_t)310,(seq_t)360,(seq_t)418,
    (seq_t)482,(seq_t)546,(seq_t)615,(seq_t)692,(seq_t)769,
    (seq_t)848,(seq_t)940
};

static seq_t *cache = NULL;      /* allocated cache */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_size = 0;    /* number of values computed (cache[0..cache_size-1]) */

/* Ensure that cache can store indices up to n (inclusive) and is zero‑filled where needed */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (cache_cap >= required) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 22;
    if (new_cap < required) new_cap = required;
    if (new_cap < 22) new_cap = 22;

    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    if (!new_cache) abort();                /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise newly allocated elements to zero */
    for (size_t i = cache_size; i < cache_cap; ++i) cache[i] = (seq_t)0;
}

/* Compute a(n) for any n >= 0, memoising results */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    if ((size_t)n < cache_size)            /* already computed */
        return cache[(size_t)n];

    /* Compute new values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        if (i < 22)                          /* initial values */
        {
            cache[i] = init_vals[i];
        }
        else                                 /* recurrence for i >= 22 */
        {
            seq_t pos = cache[i - 5]
                       + cache[i - 7] * (seq_t)3
                       + cache[i - 9] * (seq_t)2
                       + cache[i - 19]
                       + cache[i - 21];

            seq_t neg = cache[i - 2]
                       + cache[i - 12] * (seq_t)2
                       + cache[i - 14] * (seq_t)3
                       + cache[i - 16];

            cache[i] = pos - neg;            /* sequence is always positive */
        }
    }

    cache_size = (size_t)n + 1;              /* all indices < cache_size are valid */
    return cache[(size_t)n];
}
