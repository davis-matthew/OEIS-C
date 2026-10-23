/*
 * A014727  Squares of even elements in Pascal's triangle A007318.
 * https://oeis.org/A014727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (even binomial coefficient in Pascal's triangle at offset
 *            n)^2
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 102, u64 n <= 389,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists even binomial coefficients row by row (left to right). */
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Cached values of the sequence up to the largest n seen so far. */
    static seq_t *cache = NULL;   /* pointer to stored squares */
    static idx_t cache_cap = 0;   /* capacity of the cache array */
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* Position of the next even entry to generate. */
    static idx_t next_r = 0;      /* current row index */
    static idx_t next_k = 0;      /* current column index in that row */

    /* Grow the cache until it contains at least n+1 elements. */
    while (cache_len <= n) {
        /* Ensure we have a place to store the new value. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) { /* allocation failed – return 0 as a safe fallback */
                return 0;
            }
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* Scan the current row to find the next even binomial coefficient. */
        if (next_k <= next_r) {
            seq_t val = binom(next_r, next_k);
            if ((val & 1) == 0) {   /* the coefficient is even */
                cache[cache_len++] = val * val; /* store its square */
            }
            ++next_k;               /* move to the next column */
            continue;
        } else { /* finished the current row – advance to the next one */
            ++next_r;
            next_k = 0;
            continue;
        }
    }

    return cache[n];
}
