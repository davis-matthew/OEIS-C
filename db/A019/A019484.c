/*
 * A019484  Expansion of (8 + 7 x - 7 x^2 - 7 x^3)/(1 - 6 x - 7 x^2 + 5 x^3 +
 *          6 x^4).
 * https://oeis.org/A019484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + 7*a(n-2) - 5*a(n-3) - 6*a(n-4)
   with initial values a(0)=8, a(1)=55, a(2)=379, a(3)=2612 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* dynamic cache of computed terms */
static size_t cache_cap = 0;     /* cached capacity (in terms) */
static idx_t  cache_len = 0;     /* number of terms already filled */

/* initial values */
static const seq_t init_vals[4] = {
    (seq_t)8,
    (seq_t)55,
    (seq_t)379,
    (seq_t)2612
};

/* make sure cache can hold at least n+1 terms */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed < 4)            /* we always need space for the first four */
        needed = 4;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        cache_cap = needed;
    }
    if (cache_len == 0) {
        /* initialise the first four terms */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        cache_len = 4;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    if (n < cache_len)
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 6*a(i-1) + 7*a(i-2) - 5*a(i-3) - 6*a(i-4) */
        seq_t term = (seq_t)6 * cache[i - 1];
        term += (seq_t)7 * cache[i - 2];
        term -= (seq_t)5 * cache[i - 3];
        term -= (seq_t)6 * cache[i - 4];
        cache[i] = term;
    }
    return cache[n];
}
