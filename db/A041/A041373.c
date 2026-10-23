/*
 * A041373  Denominators of continued fraction convergents to sqrt(201).
 * https://oeis.org/A041373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the continued fraction of √201:
   a(0) = 1;
   a(k) = c[(k-1)%14] * a(k-1) + a(k-2)   (k ≥ 1)
   where c = {5,1,1,1,2,1,8,1,2,1,1,1,5,28}. */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* dynamic cache of computed values */
static idx_t cache_cap = 0;          /* allocated capacity of cache array     */
static idx_t cache_len = 0;          /* number of actually filled elements   */

static const int coeff[14] = {5,1,1,1,2,1,8,1,2,1,1,1,5,28};

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = 1;          /* a(0) = 1 */
        cache_len = 1;
    }

    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate required index */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);              /* free old memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise recurrence from last computed state */
    seq_t prev2 = 0;                     /* a(k-2) */
    seq_t prev1 = 1;                     /* a(k-1) for k==1 */
    if (cache_len >= 2) {                /* already have at least two elements */
        prev2 = cache[cache_len - 2];
        prev1 = cache[cache_len - 1];
    } else if (cache_len == 1) {         /* only a(0) stored */
        prev2 = 0;
        prev1 = cache[0];
    }

    /* compute new values up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t c = (seq_t)coeff[(k - 1) % 14];
        seq_t qk = c * prev1 + prev2;
        cache[k] = qk;
        prev2 = prev1;
        prev1 = qk;
    }

    cache_len = n + 1;  /* update length */
    return cache[n];
}
