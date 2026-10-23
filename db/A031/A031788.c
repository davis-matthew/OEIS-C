/*
 * A031788  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 20 ones.
 * https://oeis.org/A031788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that the period of the continued
 *            fraction of √k contains exactly 20 ones. The function computes
 *            the sequence by brute‑force search; terms are cached for
 *            subsequent calls
 * Range:     u8 none, u16 n <= 2118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* integer square root for 64‑bit values (binary search) */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, mid, sq = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            sq = 0;
            break;
        }
        if (mid > v / mid) {          /* mid*mid > v */
            hi = mid - 1;
        } else {                      /* mid*mid <= v */
            sq = mid;
            lo = mid + 1;
        }
    }
    return sq;
}

/* test whether k has exactly 20 ones in its continued‑fraction period */
static bool has_20_ones(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if ( (seq_t)a0 * (seq_t)a0 == (seq_t)k )   /* skip perfect squares */
        return false;

    idx_t m = 0, d = 1, a = a0;
    size_t ones = 0;

    while (true) {
        /* m = d*a - m */
        __int128 x = ( __int128 )d * a - m;
        m = (idx_t)x;

        /* d = (k - m*m) / d */
        __int128 y = ( __int128 )k - (__int128 )m * m;
        d = (idx_t)(y / d);

        /* a = (a0 + m) / d */
        a = (idx_t)( (a0 + m) / d );

        if (a == 1)
            ++ones;
        if (a == 2 * a0)          /* period completed */
            break;
    }
    return ones == 20;
}

/* -------------------------------------------------------------------------- */
/*  Cache of found terms                                                       */
/* -------------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of found k's */
static size_t cache_size = 0;      /* number of terms stored   */
static size_t cache_cap  = 0;      /* allocated capacity       */
static idx_t  next_k     = 1;      /* next candidate k to examine */

/* ensure that the cache contains at least 'need' terms */
static void ensure_cache(idx_t need)
{
    if (cache_size >= need)
        return;

    /* allocate or grow the cache */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_size < need) {
        /* find next valid k */
        while (true) {
            if (has_20_ones(next_k)) {
                if (cache_size >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) abort();
                }
                cache[cache_size++] = (seq_t)next_k;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*  Public function required by the build.                                  */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, but keep defined */
    ensure_cache(n);
    return cache[n - 1];
}
