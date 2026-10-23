/*
 * A031422  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted then
 *          there are a pair of central terms both equal to 9.
 * https://oeis.org/A031422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 107, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements sequence A031422 (OEIS).
 * Sequence definition: 
 *   a(n) = the n-th positive integer k such that
 *     -- sqrt(k) has an odd period in its simple continued fraction,
 *     -- and after removing the last term of that period,
 *        the two central terms of the remaining block are both 9.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root (floor) for uint64_t */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32);   /* high > sqrt(max uint64_t) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* cache for generated terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* buffer for a period of a continued fraction */
static uint32_t *period    = NULL;
static size_t    period_cap = 0;

/* the largest k already processed */
static uint64_t last_k = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, return 0 for safety */

    /* extend the cache until we have enough terms */
    while (cache_len < (size_t)n) {
        /* search for the next qualifying k */
        for (uint64_t k = last_k + 1; ; ++k) {
            uint64_t sq = isqrt_u64(k);
            if (sq * sq == k) continue;    /* skip perfect squares */

            /* compute the simple continued fraction of sqrt(k) */
            uint64_t a0   = sq;
            uint64_t m    = 0;
            uint64_t d    = 1;
            uint64_t a_val = a0;
            size_t period_len = 0;

            /* ensure we have space for the period */
            if (period_cap == 0) {
                period_cap = 128;
                period = (uint32_t *)malloc(period_cap * sizeof(uint32_t));
            }

            while (1) {
                m = d * a_val - m;
                unsigned __int128 tmp = (unsigned __int128)k - (unsigned __int128)m * m;
                d = (uint64_t)(tmp / d);
                a_val = (a0 + m) / d;

                if (period_len == period_cap) {
                    period_cap *= 2;
                    period = (uint32_t *)realloc(period, period_cap * sizeof(uint32_t));
                }
                period[period_len++] = (uint32_t)a_val;

                if (a_val == 2 * a0)   /* end of period */
                    break;
            }

            /* check the required property */
            if (period_len % 2 == 1 && period_len >= 3) {
                size_t m_idx = (period_len - 1) / 2;   /* index of the second central term
                                                          after deleting the last term   */
                if (period[m_idx - 1] == 9 && period[m_idx] == 9) {
                    /* store k in the cache */
                    if (cache_len == cache_cap) {
                        cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)k;

                    if (cache_len >= (size_t)n) {
                        last_k = k;
                        return cache[n - 1];
                    }
                }
            }
            last_k = k;
        }
    }
    return cache[n - 1];
}
