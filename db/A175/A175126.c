/*
 * A175126  a(0) = a(1) = 0, for n >= 2, a(n) = number of steps of iteration
 *          of {r - (smallest prime divisor of r)} needed to reach 0 starting
 *          at r = n.
 * https://oeis.org/A175126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 0; a(n) = 1 + a(n – lpf(n)) for n ≥ 2, where
 *            lpf(n) is the smallest prime divisor of n
 * Range:     u8 n <= 511, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/lpf.h"    /* provides idx_t lpf(idx_t n) */

static seq_t *cache = NULL;          /* a(n) values, 0 indicates uncomputed */
static size_t cache_cap = 0;         /* number of elements currently stored */

/* Make sure cache can address index n (inclusive) and zero‑initialize new slots. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure */

    /* Zero‑initialize newly grown part */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;

    /* Explicitly set the known values for n = 0,1 */
    cache[0] = 0;
    if (cache_cap > 1)
        cache[1] = 0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    ensure_cache(n);

    if (cache[n] != 0)        /* already computed */
        return cache[n];

    /* Even n: a(2k) = k */
    if ((n & ((idx_t)1)) == 0) {
        cache[n] = (seq_t)(n / 2);
        return cache[n];
    }

    /* Odd n */
    idx_t spf = lpf(n);
    if (spf == n) {          /* prime */
        cache[n] = 1;
        return cache[n];
    }

    /* Composite odd: recurse on n - spf */
    cache[n] = (seq_t)(1 + a(n - spf));
    return cache[n];
}
