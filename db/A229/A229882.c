/*
 * A229882  Squarefree oblong numbers.
 * https://oeis.org/A229882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007674(n)^2 + A007674(n) (i.e. the nth square‑free
 *            oblong number: k(k+1) with k and k+1 square‑free)
 * Range:     u8 n <= 7, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* External linkage required. */
seq_t a(idx_t n)
{
    /* Start at the first triangular index (k = 1). */
    static seq_t *cache = NULL;        /* array of computed terms */
    static idx_t  cache_len = 0;       /* number of terms stored      */
    static idx_t  cache_cap = 0;       /* allocated capacity          */
    static idx_t  next_k   = 1;        /* next k to examine           */

    if (n == 0) {
        /* The sequence is defined for n >= 1.  For n < 1 return 0. */
        return 0;
    }

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Check if both k and k+1 are square‑free. */
        if (is_squarefree((seq_t)next_k) &&
            is_squarefree((seq_t)(next_k + 1))) {
            /* Ensure enough capacity in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failed */
                cache     = tmp;
                cache_cap = new_cap;
            }
            /* Compute the oblong number k(k+1). */
            seq_t kseq = next_k;
            cache[cache_len++] = kseq * (kseq + (seq_t)1);
        }
        next_k++;
    }

    /* Return the requested term.  */
    return cache[n - 1];
}
