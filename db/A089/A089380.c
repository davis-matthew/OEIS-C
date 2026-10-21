/*
 * A089380  Number of Motzkin paths of length n with no UD, UHD, UHHD, UHHHD,
 *          ..., starting at level zero (here H=(1,0), U=(1,1), D=(1,-1)).
 * https://oeis.org/A089380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   5-term recurrence * 2*(n+2)*a(n) + 2*(-5*n-4)*a(n-1) +
 *            (13*n+2)*a(n-2) + (n-16)*a(n-3) * + 3*(-5*n+14)*a(n-4) +
 *            9*(n-4)*a(n-5) = 0 * with a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=2, a(5)=6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache        = NULL;
static size_t cache_cap    = 0;
static size_t max_computed = 0;
static const seq_t init_vals[6] = {1, 1, 1, 1, 2, 6};

static void init_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    ensure_cache((idx_t)needed, &cache, &cache_cap);
    /* Initialize known values */
    for (size_t i = 0; i < 6 && i < needed; ++i) cache[i] = init_vals[i];
    max_computed = 5;
}

seq_t a(idx_t n)
{
    if (n < 6) return init_vals[(size_t)n];

    /* Ensure cache large enough */
    if (cache == NULL) init_cache((size_t)n + 1);
    else if ((size_t)n + 1 > cache_cap) init_cache((size_t)n + 1);

    if (n <= max_computed) return cache[(size_t)n];

    /* Compute values from (max_computed+1) up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        idx_t ni = (idx_t)i;

        /* Positive sum */
        seq_t pos = (seq_t)(10 * ni + 8) * cache[i - 1];
        pos += (seq_t)(15 * ni - 42) * cache[i - 4];

        /* Negative sum */
        seq_t neg = (seq_t)(13 * ni + 2) * cache[i - 2];
        neg += (seq_t)(9 * (ni - 4)) * cache[i - 5];

        /* Handle (16 - n) term which can change sign */
        int64_t coeff3 = (int64_t)16 - (int64_t)ni;  /* may be negative */
        if (coeff3 > 0) {
            pos += (seq_t)(coeff3) * cache[i - 3];
        } else if (coeff3 < 0) {
            neg += (seq_t)(-coeff3) * cache[i - 3];
        }

        /* numerator and denominator are positive */
        seq_t num = pos - neg;
        seq_t den = (seq_t)(2 * (ni + 2));
        cache[i] = num / den;
    }

    max_computed = (size_t)n;
    return cache[(size_t)n];
}
