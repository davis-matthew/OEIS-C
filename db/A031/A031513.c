/*
 * A031513  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 15.
 * https://oeis.org/A031513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031513: Numbers k such that the continued fraction for sqrt(k)
 * has even period and if the last term of the periodic part is deleted
 * the central term is 15. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root, floor.  Works for 64‑bit unsigned integers. */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Test whether a given k satisfies the condition of the sequence. */
static bool test_k(uint64_t k)
{
    uint64_t a0 = isqrt_uint64(k);
    if (a0 * a0 == k) return false;   /* skip perfect squares */

    uint64_t m = 0, d = 1, a = a0;

    /* Store the periodic terms; we need at most 2*a0 terms. */
    size_t cap = 8, len = 0;
    uint64_t *periods = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!periods) abort();

    while (1) {
        /* m = d*a - m */
        uint64_t m_new = (uint64_t)(((__int128)d * a) - m);
        m = m_new;

        /* d = (k - m*m) / d */
        uint64_t d_new = (uint64_t)((((__int128)k - (__int128)m * m) ) / d);
        d = d_new;

        /* a = (a0 + m) / d */
        a = (uint64_t)((a0 + m) / d);

        if (len == cap) {
            cap *= 2;
            uint64_t *tmp = (uint64_t *)realloc(periods, cap * sizeof(uint64_t));
            if (!tmp) { free(periods); abort(); }
            periods = tmp;
        }
        periods[len++] = a;

        if (a == 2 * a0) break;  /* last term of the period */
    }

    bool ok = false;
    if ((len & 1) == 0) {   /* even period */
        size_t p = len / 2;
        /* central term after deletion is at index p-1 (0‑based) */
        if (periods[p - 1] == 15) ok = true;
    }

    free(periods);
    return ok;
}

/* Sequence function a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms (stored as 64‑bit values). */
    static uint64_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;

    static idx_t next_k = 2;   /* smallest k to test */

    while (cache_cnt < (size_t)n) {
        if (test_k(next_k)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!tmp) { free(cache); abort(); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = next_k;
        }
        next_k++;
    }

    return (seq_t)cache[n - 1];
}
