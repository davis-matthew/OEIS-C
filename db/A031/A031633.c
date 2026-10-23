/*
 * A031633  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 45.
 * https://oeis.org/A031633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the continued fraction for sqrt(k)
   has odd period and after deleting the last term the two
   central terms of the remaining period are both 45. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values. */
static inline idx_t uint64_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Determine whether a non‑square integer k satisfies the definition. */
static bool is_good_k(idx_t k)
{
    idx_t a0 = uint64_isqrt(k);
    if (a0 * a0 == k) return false;     /* exclude perfect squares */

    /* Generate the period using the standard algorithm. */
    idx_t m = 0, d = 1, a = a0;
    static size_t period_cap = 0;
    static idx_t *period = NULL;

    if (!period_cap) {
        period_cap = 2048;
        period = (idx_t *)malloc(period_cap * sizeof(idx_t));
        if (!period) return false;
    }

    size_t len = 0;

    for (;;) {
        /* Next values.  Use 128‑bit arithmetic for the multiplication. */
        m = d * a - m;
        __int128 t128 = ( __int128)k - (__int128)m * m;
        d = (idx_t)(t128 / d);
        a = (a0 + m) / d;

        /* Store the new partial quotient. */
        if (len == period_cap) {
            period_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(period, period_cap * sizeof(idx_t));
            if (!tmp) return false;
            period = tmp;
        }
        period[len++] = a;

        /* The last term of the period satisfies a == 2*a0. */
        if (a == 2 * a0) break;
    }

    if (len < 3) return false;          /* need at least three terms */
    if ((len & 1) == 0) return false;   /* period must be odd */

    size_t left  = (len - 2) >> 1;      /* indices in the original array   */
    size_t right = left + 1;
    return period[left] == 45 && period[right] == 45;
}

/* --------------------------------------------------------------- */
/*  Memoised generation of the sequence.                           */
/* --------------------------------------------------------------- */

static seq_t *cache   = NULL;  /* stored sequence values */
static size_t cache_len = 0;   /* number of stored values   */
static size_t cache_cap = 0;   /* allocated capacity        */
static idx_t next_candidate = 2;/* next integer to test     */

static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? (cache_cap << 1) : 8;
    if (newcap < need) newcap = need;
    seq_t *ptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!ptr) abort(); /* out of memory */
    cache = ptr;
    cache_cap = newcap;
}

/* --------------------------------------------------------------- */
/*  Requested sequence function.                                   */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* Undefined for n = 0, but ignored */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (is_good_k(next_candidate)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
