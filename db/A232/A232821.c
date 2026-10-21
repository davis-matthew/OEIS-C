/*
 * A232821  a(n) = n^(n-1) - Sum_{k=1..n-1} k^(k-1).
 * https://oeis.org/A232821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_seq_array, big_pow, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n^(n-1) - sum_{k=1..n-1} k^(k-1)  (OEIS A232821)
 *           a(1) = 1, a(2) = 1, a(3) = 6, ...
 *
 * This implementation uses a basic memoised cache of the terms
 * k^(k-1) and their running sums.  The cache is resized when a
 * larger index is requested.  All allocations are performed
 * through the utility functions that are compatible with both
 * plain integers and arbitrary‑precision integer classes.
 *
 * Dependencies:
 *   - "../utils/big_pow.h"        for big_pow
 *   - "../utils/alloc_seq_array.h" for alloc_seq_array
 *   - "../utils/free_seq_array.h"   for free_seq_array
 *
 * The function is safe for repeated calls with indices in any
 * order up to a very large maximum (e.g., n ≈ 10⁴) and runs comfortably
 * under the given time limit.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/big_pow.h"        /* big_pow(seq_t base, seq_t exp)         */
#include "../utils/alloc_seq_array.h" /* alloc_seq_array(size_t n)              */
#include "../utils/free_seq_array.h"  /* free_seq_array(seq_t *p)               */

seq_t a(idx_t n) {
    /* Static cache */
    static seq_t *terms = NULL;          /* terms[i] = i^(i-1)    */
    static seq_t *sums  = NULL;          /* sums[i]  = Σ_{k=1..i} k^(k-1) */
    static idx_t  max_done = 0;          /* largest i computed so far  */
    static size_t capacity = 0;          /* allocated length            */

    /* n is 1‑based; defensive guard for undefined offset */
    if (n == 0) {
        return 0;
    }

    /* Extend the cache if a larger n is requested */
    if (n > max_done) {
        size_t need = (size_t)n + 1;     /* need room for index n    */
        /* Handle reallocation */
        if (need > capacity) {
            /* Allocate new arrays */
            seq_t *new_terms = alloc_seq_array(need);
            seq_t *new_sums  = alloc_seq_array(need);
            if (!new_terms || !new_sums) {
                return 0; /* out‑of‑memory, return zero */
            }

            /* Copy existing data if any */
            if (terms) {
                for (idx_t i = 0; i < capacity; ++i) {
                    new_terms[i] = terms[i];
                    new_sums[i]  = sums[i];
                }
            }

            /* Free old arrays */
            if (terms) free_seq_array(terms);
            if (sums)  free_seq_array(sums);

            /* Install new arrays */
            terms    = new_terms;
            sums     = new_sums;
            capacity = need;

            /* Initialise the 0th entries which are used by future sums */
            if (capacity > 0) {
                terms[0] = 0;
                sums[0]  = 0;
            }
        }

        /* Compute new terms and cumulative sums */
        for (idx_t i = max_done + 1; i <= n; ++i) {
            seq_t t = big_pow((seq_t)i, (seq_t)(i - 1));
            terms[i] = t;
            sums[i]  = sums[i - 1] + t;
        }
        max_done = n;
    }

    /* a(n) = n^(n-1) - Σ_{k=1}^{n-1} k^(k-1) = terms[n] - sums[n-1] */
    return terms[n] - sums[n - 1];
}
