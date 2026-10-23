/*
 * A031414  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted then
 *          there are a pair of central terms both equal to 1.
 * https://oeis.org/A031414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031414: Numbers k such that the continued fraction for sqrt(k)
   has odd period and, after deleting the last term of that period, the
   two central terms are both equal to 1.  The function returns the
   n‑th such integer (1‑based).  No closed formula is known, so we search
   sequentially. */
 
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64(idx_t) */

/* Test whether k satisfies the A031414 property.  The calculation is
   performed with 128‑bit arithmetic to avoid overflow on 64‑bit k. */
static bool good_k(idx_t k)
{
    /* Skip perfect squares. */
    idx_t a0 = _floor_isqrt_u64(k);
    if (a0 * a0 == k) {
        return false;
    }

    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a   = a0;
    /* Period terms – the maximum period length for a 64‑bit integer is
       well below 4096, so a fixed array suffices. */
    static seq_t period[4096];
    idx_t len = 0;

    /* Generate the period.  The loop stops when the last term equals
       2*a0, which is guaranteed for quadratic irrationals. */
    do {
        /* m = d * a - m */
        unsigned __int128 tmp = (unsigned __int128)d * a - m;
        m = (idx_t)tmp;

        /* d = (k - m*m) / d */
        tmp = (unsigned __int128)k - (unsigned __int128)m * m;
        d = (idx_t)(tmp / d);

        /* a = (a0 + m) / d */
        a = (a0 + m) / d;

        if (len < (idx_t)sizeof(period)/sizeof(period[0])) {
            period[len++] = (seq_t)a;
        } else {
            /* Should never happen for 64‑bit k. */
            return false;
        }
    } while (a != 2 * a0);

    /* The period must be odd. */
    if ((len & 1) == 0) {
        return false;
    }

    /* After deleting the last term (2*a0), the remaining terms are
       period[0 .. len-2].  Their length is even.  The two middle terms
       should both be 1. */
    idx_t central1 = (len - 3) / 2;   /* floor((len-3)/2) */
    idx_t central2 = central1 + 1;    /* its pair */

    return period[central1] == 1 && period[central2] == 1;
}

/* Memoised generator of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  cache_cnt = 0;
    static idx_t  current_k = 2;   /* next candidate to test */

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        /* Find the next k that satisfies the property. */
        while (true) {
            if (good_k(current_k)) {
                if (cache_cnt == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = (seq_t)current_k;
                ++current_k;          /* advance to the next candidate */
                break;
            }
            ++current_k;
        }
    }

    return cache[n - 1];
}
