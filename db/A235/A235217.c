/*
 * A235217  Number of (n+1)X(2+1) 0..2 arrays with 2X2 edge jumps all no more
 *          than +1 in one of the clockwise or counterclockwise directions or
 *          both.
 * https://oeis.org/A235217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(1)=427, a(2)=8029, a(3)=145669, a(4)=2667851,
   a(5)=48751703, a(6)=891368897, a(7)=16295474229,
   a(8)=297913907755
   a(n) = 16*a(n-1) + 52*a(n-2) -188*a(n-3) -10*a(n-4) +188*a(n-5)
          -29*a(n-6) -40*a(n-7) +10*a(n-8)   (for n ≥ 9)
*/

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* stored terms a(1)...a(k), 0‑based index */
static size_t cache_cap = 0;     /* current allocation size */
static size_t cache_len = 0;     /* number of terms actually computed */

/* Ensure the cache can hold at least 'need' terms (1‑based) */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, return 0 as a safe guard */
        return (seq_t)0;

    size_t idx = (size_t)n;          /* 1‑based index in cache (0‑based) */

    /* Grow cache if necessary */
    ensure_cap(idx);

    /* If needed terms not yet computed, compute them */
    if (idx > cache_len)
    {
        /* Initial 8 terms, as constants */
        static const seq_t init[8] = {
            (seq_t)427, (seq_t)8029, (seq_t)145669, (seq_t)2667851,
            (seq_t)48751703, (seq_t)891368897, (seq_t)16295474229,
            (seq_t)297913907755
        };

        for (size_t i = cache_len; i < idx; ++i)
        {
            if (i < 8)
            {
                cache[i] = init[i];
            }
            else
            {
                /* Compute using the recurrence (positive + negative parts separately) */
                seq_t pos = 0, neg = 0;
                pos += cache[i-1] * (seq_t)16;
                pos += cache[i-2] * (seq_t)52;
                neg += cache[i-3] * (seq_t)188;
                neg += cache[i-4] * (seq_t)10;
                pos += cache[i-5] * (seq_t)188;
                neg += cache[i-6] * (seq_t)29;
                neg += cache[i-7] * (seq_t)40;
                pos += cache[i-8] * (seq_t)10;
                /* Final value: pos – neg (will be non‑negative) */
                cache[i] = pos - neg;
            }
        }
        cache_len = idx;
    }

    return cache[idx-1];
}
