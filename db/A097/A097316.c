/*
 * A097316  Chebyshev U(n,x) polynomial evaluated at x=33.
 * https://oeis.org/A097316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=66, a(n)=66*a(n-1)-a(n-2) (n≥2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 541 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_coef = 0;
static idx_t cache_len  = 0;

static void ensure_up_to(idx_t n)
{
    /* Grow capacity if needed */
    if (cache_coef <= n) {
        idx_t new_cap = (cache_coef==0) ? (n+1>2?n+1:2) : cache_coef*2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();               /* abort on allocation failure */
        cache = tmp;
        cache_coef = new_cap;
    }

    /* Initialise base cases once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)66;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)66 - cache[i-2];
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
