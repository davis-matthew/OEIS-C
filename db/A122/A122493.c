/*
 * A122493  Union of Fibonacci and triangular numbers.
 * https://oeis.org/A122493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th element of the sorted union of the Fibonacci *
 *            sequence (0,1,1,2,3,5,8,13,…) and the triangular numbers *
 *            sequence (0,1,3,6,10,15,21,…) with duplicates removed
 * Range:     u8 n <= 29, u16 n <= 379, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t)   */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*)    */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Fibonacci state: fib_prev holds F(k), fib_curr holds F(k+1) */
    static seq_t fib_prev = 0;
    static seq_t fib_curr = 1;

    /* Triangular state */
    static idx_t tri_idx = 0;                /* current triangle index */
    static seq_t tri_val = 0;                /* current triangular number */

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Generate terms until we have reached index n. */
    while (cache_len <= n) {
        /* Grow the cache if necessary. */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 8 : cache_cap << 1);
            seq_t *new_cache = alloc_seq_array(new_cap);
            if (!new_cache)          /* allocation failure – very unlikely */
                return (seq_t)0;
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Current candidates from each sequence. */
        seq_t cand_fib = fib_prev;
        seq_t cand_tri = tri_val;
        seq_t candidate = (cand_fib <= cand_tri) ? cand_fib : cand_tri;

        /* Skip duplicates that are already stored. */
        while (cache_len > 0 && candidate == cache[cache_len - 1]) {
            if (cand_fib == candidate) {
                seq_t old_prev = fib_prev;
                fib_prev = fib_curr;
                fib_curr = old_prev + fib_curr;
                cand_fib = fib_prev;
            }
            if (cand_tri == candidate) {
                ++tri_idx;
                tri_val = (seq_t)tri_idx * ((seq_t)tri_idx + (seq_t)1) / 2;
                cand_tri = tri_val;
            }
            candidate = (cand_fib <= cand_tri) ? cand_fib : cand_tri;
        }

        /* Store the unique candidate. */
        cache[cache_len++] = candidate;

        /* Advance any sequence that produced this candidate. */
        if (cand_fib == candidate) {
            seq_t old_prev = fib_prev;
            fib_prev = fib_curr;
            fib_curr = old_prev + fib_curr;
        }
        if (cand_tri == candidate) {
            ++tri_idx;
            tri_val = (seq_t)tri_idx * ((seq_t)tri_idx + (seq_t)1) / 2;
        }
    }

    return cache[n];
}
