/*
 * A031543  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 45.
 * https://oeis.org/A031543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the continued fraction for sqrt(k) has even period
 * and the term at position period_len/2 is 45.
 *
 * The function searches over consecutive integers k, skipping squares,
 * computes the continued fraction expansion of sqrt(k), and records k
 * whenever the conditions are satisfied.  Results are cached so that
 * repeated calls to a(n) for arbitrary indices run efficiently.
 * The implementation uses only integer arithmetic and __int128 for
 * intermediate products to avoid overflow.  It is valid C11 and
 * C++17 and works for any of the supported seq_t types (8, 16, 32,
 * 64‑bit unsigned or arbitrary precision).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- basic utilities -----------------------------------------------------*/

static idx_t isqrt_idx(idx_t n)
{
    /* floor(sqrt(n)) for 0 <= n <= 2^64-1 using binary search */
    idx_t lo = 0, hi = (uint64_t)1 << 32, mid;
    idx_t r = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return r;
}

/*--- helper to append to a resizable int array --------------------------*/
static void append_int(int **buf, size_t *len, size_t *cap, int val)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *buf = (int *)realloc(*buf, *cap * sizeof(int));
    }
    (*buf)[*len] = val;
    ++(*len);
}

/*--- main sequence function ---------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* last number examined */
    static idx_t last_k = 1;

    /* buffer for one continued‑fraction period */
    static int *period = NULL;
    static size_t period_cap = 0;

    while (cache_len < n) {
        ++last_k;
        idx_t k = last_k;

        if (k == 0)
            continue;                  /* avoid sqrt(0) */

        idx_t a0 = isqrt_idx(k);
        if (a0 * a0 == k)              /* skip perfect squares */
            continue;

        /* compute the period of continued fraction of sqrt(k) */
        idx_t m = 0, d = 1;
        idx_t a_term = a0;
        size_t terms_len = 0;

        for (;;) {
            /* m = d*a - m */
            m = (unsigned __int128)d * a_term - m;

            /* d = (k - m^2) / d */
            unsigned __int128 m2 = (unsigned __int128)m * m;
            d = (k - (uint64_t)m2) / d;

            /* a = floor((a0 + m)/d) */
            a_term = (a0 + m) / d;

            append_int(&period, &terms_len, &period_cap, (int)a_term);

            if (a_term == 2 * a0)
                break;
        }

        /* check the conditions */
        if ((terms_len & 1) == 0 && period[terms_len / 2 - 1] == 45) {
            /* store k into cache */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
