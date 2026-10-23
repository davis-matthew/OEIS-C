/*
 * A025944  Expansion of 1/((1-2*x)*(1-3*x)*(1-7*x)*(1-9*x)).
 * https://oeis.org/A025944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1) - 2^(n+1) + 16*a(n-1) - 63*a(n-2) Initial values:
 *            a(0)=1, a(1)=21, a(2)=292
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t pow2_val = 1;    /* 2^cache_len */
static seq_t pow3_val = 1;    /* 3^cache_len */

seq_t a(idx_t n)
{
    /* First-time initialization */
    if (!cache) {
        cache_cap = 4;                       /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;                /* out of memory */

        /* base values */
        cache[0] = 1;
        cache[1] = 21;
        cache[2] = 292;
        cache_len = 3;

        /* pow2_val and pow3_val must match 2^cache_len and 3^cache_len */
        pow2_val = 1;
        pow3_val = 1;
        for (idx_t i = 0; i < cache_len; ++i) {
            pow2_val *= 2;
            pow3_val *= 3;
        }
    }

    /* Extend cache if needed */
    if ((idx_t)n >= cache_len) {
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;               /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
            /* Update powers of 2 and 3 for exponent (i+1) */
            pow2_val *= 2;
            pow3_val *= 3;

            /* Compute recurrence only for n >= 3 */
            seq_t diff = 16 * cache[i - 1] - 63 * cache[i - 2];
            cache[i] = diff + (pow3_val - pow2_val);
        }
        cache_len = needed;
    }

    return cache[n];
}
