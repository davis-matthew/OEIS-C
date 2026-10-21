/*
 * A053478  Sum of iterates when phi, A000010, is iterated until fixed point
 *          1.
 * https://oeis.org/A053478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + a(phi(n)), a(1) = 1 * (sum of iterates of Euler's
 *            totient until 1) * Implementation memoises the results
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* provides static seq_t phi(idx_t m) */
#include <stdlib.h>

/* cache[n] holds a(n) for n ≥ 1.  cache[0] unused. */
static seq_t *cache   = NULL;
static idx_t  size    = 0;   /* allocated slots 0 … size-1 */

/* grow the cache so that index n is valid */
static void
ensure_cache(idx_t n)
{
    if (n < size)
        return;

    idx_t new_size = (n + 1) * 3 / 2;
    if (new_size < n + 1)
        new_size = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* allocation failed */

    /* initialise new slots to 0 (uncomputed).  Only new slots are zeroed. */
    for (idx_t i = size; i < new_size; ++i)
        tmp[i] = 0;

    cache = tmp;
    size  = new_size;
}

/* recursive definition with memoisation */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;      /* a(1) = 1 */

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];      /* already computed */

    /* compute phi(n) with the utility */
    seq_t phi_val = phi(n);    /* phi returns seq_t; φ(n) ≤ n */
    idx_t phi_n   = (idx_t)phi_val;

    seq_t result = (seq_t)n + a(phi_n);
    cache[n] = result;        /* memoise for future calls */
    return result;
}
