/*
 * A027439  Expansion of 1/(1 - 4*x + 5*x^2 - 3*x^3).
 * https://oeis.org/A027439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 3*a(n-3), a(0)=1, a(1)=4, a(2)=11
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static inline seq_t next_term(seq_t a1, seq_t a2, seq_t a3)
{
    /* a(n) = 4*a(n-1) -5*a(n-2) +3*a(n-3) */
    return (a1 * (seq_t)4) - (a2 * (seq_t)5) + (a3 * (seq_t)3);
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    /* Ensure enough capacity */
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first terms if not yet set */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
        if (n >= 1) {
            cache[1] = (seq_t)4;
            cache_size = 2;
        }
        if (n >= 2) {
            cache[2] = (seq_t)11;
            cache_size = 3;
        }
    } else if (cache_size == 1) {
        cache[1] = (seq_t)4;
        cache_size = 2;
    } else if (cache_size == 2) {
        cache[2] = (seq_t)11;
        cache_size = 3;
    }

    /* compute any missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = next_term(cache[i-1], cache[i-2], cache[i-3]);
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
