/*
 * A145458  Exponential transform of C(n,8) = A000581.
 * https://oeis.org/A145458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the coefficients of the exponential generating
 *            function exp(exp(x)*x^8/8!), i.e. the exponential transform of
 *            C(n,8). Recurrence used: a(0) = 1 a(n) = 0 for 1 ≤ n < 8 a(n) =
 *            Σ_{k=8}^n C(n−1, k−1) · C(k,8) · a(n−k) for n ≥ 8
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 21, u64 n <= 31, u128 n <= 48,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* External integer types supplied by the build */
extern size_t _floor_isqrt_u64(idx_t n);

/* ensure suggested by build utilities (may be provided) */

static seq_t *cache = NULL;
static size_t cap = 0;
static size_t computed = (size_t)-1;      /* -1 means nothing computed yet */

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cap)
        return;
    size_t new_cap = cap ? cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    for (size_t i = cap; i < new_cap; ++i)
        cache[i] = 0;
    cap = new_cap;
}

static seq_t *binom8 = NULL;
static size_t binom_cap = 0;

/* enlarge binom8 up to at least n (inclusive) */
static void ensure_binom(idx_t n)
{
    if ((size_t)n < binom_cap)
        return;
    size_t new_cap = binom_cap ? binom_cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    binom8 = (seq_t*)realloc(binom8, new_cap * sizeof(seq_t));

    /* initialise new entries */
    for (size_t k = binom_cap; k < new_cap; ++k) {
        if (k < 8)
            binom8[k] = 0;
        else if (k == 8)
            binom8[8] = 1;
        else
            binom8[k] = binom8[k - 1] * k / (k - 8);
    }
    binom_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (computed == (size_t)-1) {          /* first call */
        ensure_cache(0);
        cache[0] = 1;
        computed = 0;
    }

    ensure_cache(n);
    if ((size_t)n <= computed)
        return cache[n];

    ensure_binom(n);

    for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
        seq_t val = 0;
        if (i >= 8) {
            /* compute initial comb = C(i-1,7) */
            seq_t comb = 1;
            for (idx_t t = 0; t < 7; ++t)
                comb *= (i - 1 - t);
            comb /= 5040;   /* 7! */

            for (idx_t k = 8; k <= i; ++k) {
                seq_t term = comb * binom8[k] * cache[i - k];
                val += term;
                if (k < i)   /* update comb to C(i-1, k) for next k+1 */
                    comb = comb * (i - k) / k;
            }
        } else {
            /* for 1 ≤ i < 8, a(i) = 0 already; a(0)=1 handled above */
            val = 0;
        }
        cache[i] = val;
    }

    computed = (size_t)n;
    return cache[n];
}
