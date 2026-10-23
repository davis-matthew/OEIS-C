/*
 * A031421  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted then
 *          there are a pair of central terms both equal to 8.
 * https://oeis.org/A031421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A031421 – Numbers k such that the continued fraction for sqrt(k)
    has odd period and, after deleting the last term of that period,
    the two central terms are both equal to 8. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Check whether k satisfies the A031421 condition. */
static bool is_valid_k(idx_t k)
{
    if (k == 0) return false;
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* perfect square */

    /* Small fixed array – period length for the numbers of interest
       never exceeds 8192.  */
    enum { MAX_PERIOD = 8192 };
    static idx_t period[MAX_PERIOD];
    size_t len = 0;

    __uint128_t m = 0, d = 1, a = a0;

    while (true) {
        m = d * a - m;
        d = (__uint128_t)(k - (__uint128_t)m * m) / d;
        a = (a0 + m) / d;

        if (len == MAX_PERIOD) return false;   /* safety */
        period[len++] = (idx_t)a;

        if (a == 2 * a0) break;                /* period terminates */
    }

    if (len < 3 || (len & 1) == 0) return false;  /* need odd length >=3 */

    size_t size = len - 1;                       /* after dropping last term */
    size_t mid1 = size / 2 - 1;
    size_t mid2 = size / 2;
    return (period[mid1] == 8 && period[mid2] == 8);
}

/* Static cache of found terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  last_scanned = 1;    /* highest k checked */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for undefined index */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        last_scanned++; /* next candidate k */
        if (is_valid_k(last_scanned)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failure – abort */
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_scanned;
        }
    }
    return cache[n - 1];
}
