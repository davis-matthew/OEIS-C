/*
 * A086911  a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)).
 * https://oeis.org/A086911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |prime(n)-a(n-1)| + |prime(n)-a(n-2)|, a(1)=1, a(2)=1
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;      /* 1‑based cache of computed a(n) */
static idx_t cache_cap = 0;      /* number of elements allocated (including index 0) */

static seq_t abs_diff(seq_t x, seq_t y) { return (x > y) ? (x - y) : (y - x); }

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 3;                         /* positions 0,1,2 */
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
    }

    /* expand cache if necessary */
    if (n >= cache_cap) {
        size_t old_cap = (size_t)cache_cap;
        size_t new_cap = (size_t)n + 1;        /* 1‑based index, so +1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
        /* zero‑initialise new slots */
        for (size_t i = old_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    }

    /* compute terms up to n (in increasing order) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t prime_val = (seq_t)get_prime(i);
        seq_t d1 = abs_diff(prime_val, cache[i - 1]);
        seq_t d2 = abs_diff(prime_val, cache[i - 2]);
        cache[i] = d1 + d2;
    }

    return cache[n];
}
