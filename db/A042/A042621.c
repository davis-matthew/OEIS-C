/*
 * A042621  Denominators of continued fraction convergents to sqrt(839).
 * https://oeis.org/A042621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(839).
 *            a(0) = 1, a(1) = 1; for n >= 2 let k = (n-1) mod 4 and c = {1,
 *            27, 1, 56}. Then a(n) = c[k] * a(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached denominators */
static idx_t  cache_size = 0;    /* number of entries in cache */

/* static helper to generate terms up to `up_to` */
static void
ensure_size(idx_t up_to)
{
    if (up_to < cache_size)
        return;                     /* already available */

    idx_t new_size = up_to + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(1);                    /* allocation failed */
    cache = tmp;

    /* base cases */
    if (up_to >= 0)
        cache[0] = 1;
    if (up_to >= 1)
        cache[1] = 1;

    static const int cycle[4] = {1, 27, 1, 56};

    /* start computing from the first missing index */
    idx_t start = (cache_size == 0) ? 2 : cache_size;
    if (start < 2)
        start = 2;                  /* ensure we don't recompute 0,1 */

    for (idx_t i = start; i <= up_to; ++i) {
        seq_t ak = (seq_t) cycle[(i - 1) & 3];   /* (i-1) % 4 */
        cache[i] = ak * cache[i - 1] + cache[i - 2];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return cache[n];
}
