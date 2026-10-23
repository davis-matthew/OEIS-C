/*
 * A031605  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 17.
 * https://oeis.org/A031605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031605: Numbers k such that the continued fraction for sqrt(k)
   has odd period and, after deleting the last term of the periodic
   part, the two central terms are both 17. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for 64‑bit unsigned integers */
static inline uint64_t isqrt_u64(uint64_t x) {
    if (x < 2) return x;
    uint64_t lo = 0, hi = x >> 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* test whether n is a perfect square */
static inline bool is_square_u64(uint64_t n) {
    uint64_t r = isqrt_u64(n);
    return (unsigned __int128)r * r == n;
}

/* check whether k satisfies the A031605 property */
static inline bool candidate_k(uint64_t k) {
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* skip perfect squares */

    uint64_t m = 0, d = 1, a = a0;
    /* period will never exceed 8192 for the ranges of interest */
    static uint64_t period[8192];
    int len = 0;

    for (;;) {
        /* m = d*a - m */
        m = d * a - m;
        /* d = (k - m*m)/d */
        unsigned __int128 m2 = (unsigned __int128)m * m;
        d = (k - (uint64_t)m2) / d;
        /* a = (a0 + m) / d */
        a = (a0 + m) / d;
        /* break when the final term would be 2*a0 (period ends) */
        if (d == 1 && a == 2 * a0) break;
        period[len++] = a;
    }

    /* the full period length must be even → odd original period */
    if ((len & 1) != 0) return false;

    int mid1 = len / 2 - 1;
    int mid2 = len / 2;
    return period[mid1] == 17 && period[mid2] == 17;
}

/* cached values of the sequence */
static seq_t *seq_cache = NULL;          /* array of seq_t items */
static size_t cache_len  = 0;            /* number of stored items */
static size_t cache_cap  = 0;            /* allocated capacity */

/* helper: grow the cache to required size */
static void ensure_cache_capacity(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = need * 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – panic silently by resetting cache */
        free(seq_cache);
        seq_cache = NULL;
        cache_cap = cache_len = 0;
        return;
    }
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = seq_cache[i];
    free(seq_cache);
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* external linkage as required */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* make sure cache holds at least n values */
    if (cache_len < (size_t)n) {
        ensure_cache_capacity(n);
        static uint64_t next_k = 2;       /* start scanning from 2 */
        while (cache_len < (size_t)n) {
            while (true) {
                uint64_t k = next_k++;
                if (candidate_k(k)) {
                    seq_cache[cache_len++] = (seq_t)k;
                    break;
                }
            }
        }
    }
    return seq_cache[n - 1];
}
