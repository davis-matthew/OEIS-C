/*
 * A162476  Expansion of (1/(1-x))*c(x/(1-x)^4), c(x) the g.f. of A000108.
 * https://oeis.org/A162476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=2, a(2)=8, a(3)=39
   a(n) = [(4n-3)(4n-5)a(n-1) - 3(4n²-14n+11)a(n-2)
            - (-8n²+40n-27)a(n-3) - (2n-1)(n-6)a(n-4)] / [(n+1)(2n-3)]   (n≥4)
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;   /* cache for computed terms */
static size_t cache_cap = 0;      /* allocated capacity (in terms of elements) */
static size_t cache_next = 0;     /* next index to compute (i.e. number of computed terms) */

/* Ensure that the cache is large enough to hold indices up to 'need' inclusive. */
static void ensure_cache(idx_t need)
{
    if (need < cache_cap)
        return;
    size_t new_cap = need + 1;
    if (new_cap < 4)
        new_cap = 4;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_next)
        return seq_cache[n];

    /* First time: allocate and initialise base values. */
    if (cache_next == 0) {
        ensure_cache(3);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)8;
        seq_cache[3] = (seq_t)39;
        cache_next = 4; /* indices 0..3 are computed */
    }

    /* Make sure we have enough room for 'n' and recompute from cache_next upwards. */
    ensure_cache(n);
    for (idx_t k = cache_next; k <= n; ++k) {
        if (k == 0) {
            seq_cache[k] = (seq_t)1;
        } else if (k == 1) {
            seq_cache[k] = (seq_t)2;
        } else if (k == 2) {
            seq_cache[k] = (seq_t)8;
        } else if (k == 3) {
            seq_cache[k] = (seq_t)39;
        } else {
            /* Recurrence for k >= 4 */
            /* Coefficients */
            int64_t ccoef = -8 * (int64_t)k * k + 40 * (int64_t)k - 27;
            int64_t dcoef = (int64_t)(2 * k - 1) * (k - 6);

            /* Compute terms using unsigned multiplication.  Absolute values are used
               to avoid signed overflow, and signs are handled by the recurrence's
               subtraction or addition logic. */
            seq_t coef1 = (seq_t)((4 * k - 3) * (4 * k - 5));   /* (4n-3)(4n-5) */
            seq_t t1 = seq_cache[k - 1] * coef1;

            seq_t coef2 = (seq_t)(3 * (4 * k * k - 14 * k + 11)); /* 3(4n^2-14n+11) */
            seq_t t2 = seq_cache[k - 2] * coef2;

            seq_t term3 = seq_cache[k - 3] * (seq_t)( (ccoef >= 0) ? ccoef : -ccoef );
            seq_t term4 = seq_cache[k - 4] * (seq_t)( (dcoef >= 0) ? dcoef : -dcoef );

            seq_t numerator = t1;
            numerator -= t2;
            if (ccoef >= 0)
                numerator -= term3;
            else
                numerator += term3;
            if (dcoef >= 0)
                numerator -= term4;
            else
                numerator += term4;

            seq_t denom = (seq_t)((k + 1) * (2 * k - 3));
            seq_cache[k] = numerator / denom;
        }
    }
    cache_next = n + 1;
    return seq_cache[n];
}
