/*
 * A038838  Numbers that are divisible by the square of an odd prime.
 * https://oeis.org/A038838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer divisible by the square of an
 *            odd prime
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_square_odd_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_square_odd_prime.h"

/* Static cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t scan_cur = 1;

/* Return the n-th term of A038838 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Grow cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next integer that satisfies the condition. */
        seq_t cand = scan_cur;
        while (!has_square_odd_prime(cand)) {
            cand++;
        }

        /* Extend the cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!newp) {
                /* Allocation failed – abort */
                exit(1);
            }
            cache = newp;
            cache_cap = new_cap;
        }

        /* Store the found term. */
        cache[cache_len++] = cand;

        /* Prepare for next search. */
        scan_cur = cand + 1;
    }

    return cache[n - 1];
}
