/*
 * A020373  Numbers k such that the continued fraction for sqrt(k) has period
 *          34.
 * https://oeis.org/A020373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th positive integer k
   for which the period of the continued fraction of sqrt(k) equals 34.
   The function builds the sequence lazily, caching computed values. */

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------*/
/* 64‑bit integer square root (floor) – can use no external   */
/* implementation, so we provide a small binary search.       */
/*------------------------------------------------------------*/
static inline uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/*------------------------------------------------------------*/
/* Period length of the continued fraction for sqrt(k)        */
/* for a non‑square integer k.                                 */
/*------------------------------------------------------------*/
static inline uint64_t period_len_of_sqrt(uint64_t k)
{
    uint64_t a0 = floor_sqrt_u64(k);
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    uint64_t cnt = 0;

    do {
        m = d * a - m;
        unsigned __int128 m2 = (unsigned __int128)m * m;
        unsigned __int128 diff = (unsigned __int128)k - m2;
        d = (uint64_t)(diff / d);
        a = (a0 + m) / d;
        ++cnt;
    } while (a != 2 * a0);

    return cnt;
}

/*------------------------------------------------------------*/
/* Main sequence function                                      */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;   /* pointer to stored seq_t values   */
    static size_t cache_len  = 0; /* number of terms stored          */
    static size_t cache_cap  = 0; /* allocated array size (in seq_t)  */
    static uint64_t next_k  = 2;  /* next candidate to test          */

    /* Ensure we have at least n terms stored */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;

        /* Skip perfect squares */
        uint64_t r = floor_sqrt_u64(k);
        if ((unsigned __int128)r * r == k)
            continue;

        /* Accept if continued‑fraction period equals 34 */
        if (period_len_of_sqrt(k) == 34) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                /* allocation failure   */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
