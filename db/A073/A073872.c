/*
 * A073872  Smallest positive triangular number not included earlier that is a
 *          multiple of n (a permutation of the triangular numbers).
 * https://oeis.org/A073872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   divisible_by_n, resize_a_cache, resize_used_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073872: a(n) is the smallest unused triangular number that is
   a multiple of n.  The function builds the permutation by greedily
   selecting, for each n, the first triangular number not yet seen
   that is divisible by n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisible_by_n.h"
#include "../utils/resize_a_cache.h"
#include "../utils/resize_used_k.h"

/* Cached values of a(n), bookkeeping of used triangular indices,
   and the largest n that has been computed. */
static size_t a_cache_len = 0;
static seq_t *a_cache = NULL;
static size_t used_k_len = 0;
static bool *used_k = NULL;
static idx_t max_n_computed = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* 1‑offset sequence            */

    /* Ensure the cache can hold the requested term. */
    resize_a_cache(&a_cache_len, &a_cache, n + 1);

    /* Compute terms up to n. */
    while (max_n_computed < n)
    {
        ++max_n_computed;
        idx_t cur_n = max_n_computed;
        idx_t k = 1;

        /* Find the smallest unused k whose triangular number is a multiple
           of the current n. */
        while (true)
        {
            /* Make sure used_k[k] exists. */
            if ((size_t)k >= used_k_len)
                resize_used_k(&used_k_len, &used_k, (size_t)k + 1);

            if (!used_k[k] && divisible_by_n(k, cur_n))
                break;
            ++k;
        }

        /* Mark this k as used. */
        if ((size_t)k >= used_k_len)
            resize_used_k(&used_k_len, &used_k, (size_t)k + 1);
        used_k[k] = true;

        /* Store the triangular number for this index. */
        a_cache[cur_n] = (seq_t)k * ((seq_t)k + 1) / 2;
    }

    return a_cache[n];
}
