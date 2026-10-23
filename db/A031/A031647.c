/*
 * A031647  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 59.
 * https://oeis.org/A031647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A031647: Numbers k such that the continued fraction for sqrt(k)
 * has odd period and, after deleting the last term of the periodic part,
 * the two central terms are both 59.
 * The function computes a(n) on demand, memoising results in a static
 * dynamic array so that calls in arbitrary order are supported.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer sqrt for 64‑bit unsigned numbers */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t lo = 0, hi = (uint64_t)1 << 32;          /* hi > sqrt(UINT64_MAX) */
    while (lo + 1 < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) lo = mid;
        else hi = mid;
    }
    if ((unsigned __int128)hi * hi <= n) return hi;
    return lo;
}

/* a(n) */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;   /* last k examined */

    /* initialise cache on first call or if empty */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t k = ++last_checked;               /* examine next integer */

        /* skip perfect squares */
        uint64_t a0 = isqrt_u64(k);
        if (a0 * a0 == k) continue;

        /* compute the period of the continued fraction of sqrt(k) */
        static int period[1024];
        size_t L = 0;

        uint64_t m = 0, d = 1, a = a0;
        for (;;) {
            /* m = d*a - m */
            unsigned __int128 prod = (unsigned __int128)d * a;
            m = (uint64_t)(prod - m);

            /* d = (k - m*m) / d */
            unsigned __int128 num = (unsigned __int128)k - (unsigned __int128)m * m;
            d = (uint64_t)(num / d);

            /* a = (a0 + m) / d */
            a = (a0 + m) / d;

            period[L++] = (int)a;
            if (a == 2 * a0) break;   /* end of period */
            if (L >= sizeof(period)/sizeof(period[0])) break; /* safety */
        }

        if ((L & 1) && L >= 3) {                   /* period must be odd */
            size_t mid = (L - 1) / 2;                 /* index after deletion */
            if (mid > 0 && period[mid - 1] == 59 && period[mid] == 59) {
                /* store candidate */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[(size_t)n - 1];
}
