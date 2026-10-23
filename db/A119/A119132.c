/*
 * A119132  Triangular numbers composed of digits {1,5,6}.
 * https://oeis.org/A119132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000217(A119133(n)) (nth triangular number whose digits
 *            are only 1,5,6)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th triangular number composed only of the digits {1,5,6}. */
seq_t a(idx_t n)
{
    /* Static cache of found triangular numbers. */
    static seq_t *tri_cache = NULL;
    static idx_t tri_cache_size = 0;   /* number of filled entries */
    static idx_t tri_cache_cap  = 0;   /* allocated capacity */
    static idx_t last_k = 0;           /* last k processed (triangular index) */

    /* Grow the cache lazily. */
    if (tri_cache == NULL) {
        tri_cache_cap = 64;
        tri_cache = (seq_t *)malloc(sizeof(seq_t) * tri_cache_cap);
    }

    /* Generate triangles until we have at least n of them. */
    while (tri_cache_size < n) {
        /* Next triangular index. */
        ++last_k;
        /* Compute t = last_k * (last_k + 1) / 2.  Use seq_t arithmetic to avoid overflow. */
        seq_t k = (seq_t)last_k;
        seq_t t = k * (k + (seq_t)1) / (seq_t)2;

        /* Check if all digits of t belong to {1,5,6}. */
        bool good = true;
        seq_t tmp = t;
        while (tmp > (seq_t)0) {
            seq_t d = tmp % (seq_t)10;
            if (d != (seq_t)1 && d != (seq_t)5 && d != (seq_t)6) {
                good = false;
                break;
            }
            tmp = tmp / (seq_t)10;
        }

        if (good) {
            /* Store t in the cache. */
            if (tri_cache_size == tri_cache_cap) {
                tri_cache_cap *= 2;
                tri_cache = (seq_t *)realloc(tri_cache, sizeof(seq_t) * tri_cache_cap);
            }
            tri_cache[tri_cache_size++] = t;
        }
    }

    return tri_cache[n - 1];
}
