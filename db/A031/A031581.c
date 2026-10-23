/*
 * A031581  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 83.
 * https://oeis.org/A031581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th integer k for which the continued fraction of sqrt(k)
   has an even period and, after deleting the last term (2*a0),
   the unique central term of the remaining period equals 83.
   The function builds the sequence lazily and caches results
   for repeated calls. */

/* required headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* -------------  helper: integer square root ---------------------- */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* use 128‑bit arithmetic to avoid overflow */
        if (((__int128)mid * mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* -------------  test whether a given k satisfies the condition -- */
static bool qualifies(idx_t k)
{
    /* skip perfect squares */
    idx_t a0 = isqrt_idx(k);
    if (a0 * a0 == k)
        return false;

    /* store period terms in a dynamic array */
    static idx_t *period = NULL;
    static size_t period_cap = 0;
    static size_t period_len = 0;

    /* allocate or enlarge buffer as needed */
    if (period == NULL) {
        period_cap = 64;
        period = (idx_t *)malloc(period_cap * sizeof(idx_t));
        if (!period) return false; /* allocation failure, treat as not qualifying */
    }

    period_len = 0;
    idx_t m = 0, d = 1, a = a0;

    while (1) {
        /* compute next term */
        m = d * a - m;
        __int128 diff = (__int128)k - (__int128)m * m;
        d = (idx_t)(diff / d);
        a = (a0 + m) / d;

        /* store a */
        if (period_len == period_cap) {
            period_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(period, period_cap * sizeof(idx_t));
            if (!tmp) return false;   /* allocation failure */
            period = tmp;
        }
        period[period_len++] = a;

        /* period ends when a equals 2*a0 */
        if (a == (a0 << 1))
            break;
    }

    /* period length must be even */
    if (period_len & 1u)
        return false;

    /* central term after deleting the last term */
    size_t mid_idx = (period_len - 2u) >> 1;
    return period[mid_idx] == 83u;
}

/* --------------  sequence function -------------------------------- */
seq_t a(idx_t n)
{
    /* cache for previously found terms */
    static idx_t cache_count = 0;
    static idx_t cache_cap = 0;
    static seq_t *cache = NULL;
    static idx_t next_k = 1;   /* next k to test */

    if (n == 0u)                 /* undefined for n=0, return 0 */
        return 0;

    /* extend cache until it contains at least n terms */
    while (cache_count < n) {
        if (qualifies(next_k)) {
            /* ensure cache has room */
            if (cache_count == cache_cap) {
                cache_cap = cache_cap == 0 ? 64 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) break;   /* allocation failure */
                cache = tmp;
            }
            cache[cache_count++] = (seq_t)next_k;
        }
        next_k++;
    }

    /* if we failed to generate enough terms (unlikely), return 0 */
    if (cache_count < n)
        return 0;
    return cache[n - 1];
}
