/*
 * A079963  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={1,2}.
 * https://oeis.org/A079963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079963: a(n) = a(n‑1)+a(n‑4)+a(n‑5)+a(n‑6)
   with a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=4;
   Index 0 is the first term. */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* static cache of already computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains values up to index n, computing any
   missing terms using the recurrence.  The cache may grow to exactly
   n+1 elements; growth is inexpensive for the expected n (~10⁴). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)          /* already computed up to n */
        return;

    /* remember the previous length before reallocation */
    idx_t old_len = cache_len;
    idx_t needed  = n + 1;

    /* grow the memory block to the needed size */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    assert(tmp);                 /* abort on allocation failure */
    cache = tmp;
    cache_len = needed;

    /* initialise new slots to zero – they will be overwritten */
    for (idx_t i = old_len; i < needed; ++i)
        cache[i] = 0;

    /* compute missing terms from old_len up to n */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i <= 5) {            /* initial values */
            static const seq_t init[6] = {1, 1, 1, 1, 2, 4};
            cache[i] = init[i];
        } else {                 /* recurrence for i >= 6 */
            cache[i] = cache[i-1] + cache[i-4] + cache[i-5] + cache[i-6];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
