/*
 * A075371  Smallest squarefree number greater than the previous term such
 *          that the every partial sum is squarefree.
 * https://oeis.org/A075371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0 a(n) = smallest integer > a(n‑1) that is
 *            squarefree, and such that the cumulative sum S(n)=∑_{k=0..n}
 *            a(k) is squarefree for all k≤n
 * Range:     u8 n <= 14, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a persistent cached sequence and grows the cache as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x); */

static seq_t *seq_cache = NULL;   /* dynamic array of cached terms          */
static size_t  cache_cap = 0;     /* allocated capacity (number of slots)  */
static idx_t   cache_len = 0;     /* number of terms computed so far       */
static seq_t   cumulative_sum = 0;/* sum of all computed terms */

/* Ensure that terms a(0)…a(n) are computed and stored in seq_cache. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                       /* already available */

    /* Allocate more space if necessary.  We need capacity > n. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                   /* allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute and cache terms up to n inclusive. */
    for (; cache_len <= n; ++cache_len) {
        seq_t cand;
        if (cache_len == 0) {
            cand = 1;                 /* a(0) = 1 */
        } else {
            cand = seq_cache[cache_len - 1] + (seq_t)1;   /* start > previous */
            while (!is_squarefree(cand) ||
                   !is_squarefree(cumulative_sum + cand)) {
                cand = cand + (seq_t)1;             /* try next integer */
            }
        }
        seq_cache[cache_len] = cand;
        cumulative_sum += cand;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
