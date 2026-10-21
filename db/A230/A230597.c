/*
 * A230597  Numbers n such that (the binary weight of n) = (the ternary weight
 *          of n).
 * https://oeis.org/A230597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer such that popcount₂(n) equals the count
 *            of 1's in its base‑3 representation
 * Range:     u8 n <= 28, u16 n <= 1774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _eq_weight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_eq_weight.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, return 0 as a default */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next  = 1;  /* next integer to test */

    /* initialise cache with a(1) = 0 */
    if (!cache) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;
        cache[0] = (seq_t)0;  /* a(1) */
        len   = 1;
        next  = 1;
    }

    /* extend cache until it contains n terms */
    idx_t target = n - 1;          /* zero‑based index */
    while (len <= (size_t)target) {
        idx_t cand = next;
        /* find the next integer whose binary popcount equals its ternary
           popcount of digit '1' */
        while (1) {
            if (_eq_weight(cand)) {
                if (len == cap) {
                    size_t newcap = cap * 2;
                    seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!p) {
                        free(cache);
                        return 0;
                    }
                    cache = p;
                    cap   = newcap;
                }
                cache[len++] = (seq_t)cand;
                next = cand + 1;
                break;
            }
            ++cand;
        }
    }

    return cache[target];
}
