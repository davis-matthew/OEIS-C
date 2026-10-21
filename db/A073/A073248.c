/*
 * A073248  Squarefree numbers k such that k+1 is also squarefree, but k-1 and
 *          k+2 are not.
 * https://oeis.org/A073248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 4694, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Squarefree integers k such that k+1 is also squarefree,
   but k-1 and k+2 are not.  The first term is a(1)=10. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) {                     /* defined only for n>=1 */
        return 0;
    }

    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 1;        /* next integer to test */

    /* grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t k_u = next_k++;
        seq_t k_seq = (seq_t)k_u;

        if (is_squarefree(k_seq) &&
            is_squarefree(k_seq + 1) &&
            !is_squarefree(k_seq - 1) &&
            !is_squarefree(k_seq + 2)) {

            /* ensure enough capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) {
                    /* allocation failed; abort to avoid undefined behaviour */
                    exit(EXIT_FAILURE);
                }
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_seq;   /* implicit conversion to seq_t */
        }
    }

    return cache[n - 1];
}
