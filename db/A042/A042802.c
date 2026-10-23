/*
 * A042802  Numerators of continued fraction convergents to sqrt(932).
 * https://oeis.org/A042802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(932), with a(0)
 *            = floor(sqrt(932)) = 30; recurrence: h[-2] = 0, h[-1] = 1, h(k)
 *            = a(k)*h(k-1)+h(k-2) for k >= 0 where a(0)=30, a(k) (k>=1) is
 *            periodic with period 22:
 *            [1,1,8,4,1,1,2,1,1,1,14,1,1,1,2,1,1,4,8,1,1,60]
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Precomputed period of continued fraction coefficients for sqrt(932) */
static const unsigned short period[22] = {
    1, 1, 8, 4, 1, 1, 2, 1, 1, 1,
    14, 1, 1, 1, 2, 1, 1, 4, 8, 1, 1, 60
};

#define PERIOD_LEN (sizeof(period)/sizeof(period[0]))

/* Cache of previously computed numerators */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* In case of allocation failure, abort as per build policy */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Compute numerators up to index n, caching the results */
    if (n >= cache_len) {
        ensure_cache(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t hi;
            if (i == 0) {
                /* Base convergent numerator */
                hi = 30;
            } else if (i == 1) {
                /* a1 * h0 + 1 */
                seq_t ai = period[0];
                hi = ai * cache[0] + 1;
            } else {
                seq_t ai = period[(i - 1) % PERIOD_LEN];
                seq_t h_prev = cache[i - 1];
                seq_t h_prev2 = cache[i - 2];
                hi = ai * h_prev + h_prev2;
            }
            cache[i] = hi;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
