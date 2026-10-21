/*
 * A051408  a(n+1) = a(n) + sum of digits of (a(n)^3).
 * https://oeis.org/A051408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n+1) = a(n) + digit_sum(a(n)^3), a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS offset: 1 (a(1) is the first term) */

#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;       /* cache[0] unused, cache[1] .. cache[max] store terms */
static size_t cache_cap = 0;      /* current capacity of cache (index max + 1) */
static idx_t max_comp = 0;        /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* behaviour for n < 1 is undefined */

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, (size_t)(n + 1));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)3;
        } else {
            seq_t prev = cache[i - 1];
            seq_t cube = prev * prev * prev;          /* a(i-1)^3 */
            seq_t ds   = digit_sum(cube);            /* sum of decimal digits */
            cache[i] = prev + ds;                     /* a(i) */
        }
    }
    max_comp = n;

    return cache[n];
}
