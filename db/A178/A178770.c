/*
 * A178770  a(0) = a(1) = a(2) = a(3) = a(4) = 1, thereafter a(n) = a(a(n-1))
 *          + a(n-a(n-3)).
 * https://oeis.org/A178770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=1; for n≥5, a(n)=a(a(n-1))+a(n-a(n-3))
 * Range:     u8 n <= 504, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_cap = 0;      /* current capacity of cache */

static void grow_cache(idx_t n)
{
    /* ensure cache can hold indices up to n */
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();

    /* initialise newly allocated entries to zero (uncomputed) */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return 1;                     /* a(0) … a(4) = 1 */

    grow_cache(n);

    if (cache[n] != 0)
        return cache[n];              /* cached value */

    /* recursive computation with memoisation */
    seq_t val_n1 = a(n - 1);                 /* a(n‑1)    */
    seq_t term1  = a((idx_t)val_n1);         /* a(a(n‑1)) */

    seq_t val_n3 = a(n - 3);                 /* a(n‑3)    */
    idx_t idx2   = n - (idx_t)val_n3;        /* n‑a(n‑3)  */
    seq_t term2  = a(idx2);                  /* a(n‑a(n‑3)) */

    seq_t result = term1 + term2;
    cache[n] = result;
    return result;
}
