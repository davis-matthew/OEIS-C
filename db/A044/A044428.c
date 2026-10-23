/*
 * A044428  Numbers n such that string 9,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose decimal representation contains
 *            the substring "96" while that of n-1 does not. This definition
 *            is not expressible as a closed form, so the function computes
 *            terms by incremental search and caches results
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains96
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains96.h"

static seq_t *cache = NULL;      /* array of computed terms    */
static idx_t  cache_len = 0;     /* number of terms stored     */
static idx_t  cache_cap = 0;     /* current allocation capacity */

/* External function implementing OEIS A044428 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0; behaviour does not matter */

    /* Grow cache until it contains the n-th term */
    while (cache_len < n)
    {
        seq_t candidate = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;

        /* Find the next suitable number */
        for (;; ++candidate)
        {
            if (candidate >= 96 && contains96(candidate) && !contains96(candidate - 1))
            {
                /* Expand cache if needed */
                if (cache_len == cache_cap)
                {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = candidate;
                break; /* next term needed, if any */
            }
        }
    }

    return cache[n - 1];
}
