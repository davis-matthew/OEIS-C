/*
 * A070871  a(n) = A002487(n) * A002487(n+1) (Conway's alimentary function).
 * https://oeis.org/A070871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, look
 * Formula:   a(n) = A002487(n) * A002487(n+1) (Conway's alimentary function)
 * Range:     u8 n <= 84, u16 n <= 5289, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that the Stern sequence is computed up to index `upto`. */
static void ensure_cache(idx_t upto)
{
    if (upto < cache_size) return;
    idx_t new_sz = upto + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) return;  /* allocation failed – leave the cache as is */

    /* initialize base values if not already present */
    if (new_sz > 0) tmp[0] = 0;
    if (new_sz > 1) tmp[1] = 1;

    idx_t start = cache_size > 1 ? cache_size : 2;
    for (idx_t i = start; i < new_sz; ++i)
    {
        if ((i & 1) == 0)          /* even index: A002487(2k) = A002487(k) */
            tmp[i] = tmp[i >> 1];
        else                       /* odd index:  A002487(2k+1) = A002487(k)+A002487(k+1) */
            tmp[i] = tmp[i >> 1] + tmp[(i >> 1) + 1];
    }

    cache = tmp;
    cache_size = new_sz;
}

/* Returns the n-th term of A070871 (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; defensive handling */
    ensure_cache(n + 1);           /* we need values up to A002487(n+1) */
    return cache[n] * cache[n + 1];
}
