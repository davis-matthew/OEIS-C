/*
 * A044474  Numbers n such that string 3,1 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑5 representation of n contains the
 * substring "31" but the base‑5 representation of n+1 does not.
 *
 * The function generates terms by scanning integers and storing those that
 * satisfy the condition. Results are memoised so any subsequent call with a
 * larger or smaller index can be answered quickly.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_31.h"

seq_t a(idx_t n)
{
    static seq_t *cache        = NULL;  /* stored sequence terms   */
    static size_t cache_size   = 0;      /* number of terms cached  */
    static size_t cache_cap    = 0;      /* allocated capacity      */
    static idx_t  next_candidate = 1;    /* next integer to test    */

    if (n == 0) return 0;  /* sequence is defined from n=1 */

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        /* Find the next integer that satisfies the definition. */
        while (true) {
            seq_t cur = (seq_t)next_candidate;
            if (has_31(cur) && !has_31(cur + (seq_t)1)) {
                /* Ensure there is room in the cache. */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) return 0;  /* allocation failure */
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = cur;
                break;  /* advance to next candidate integer */
            }
            ++next_candidate;
        }
        ++next_candidate;  /* skip the current term so we test the next integer next time */
    }

    return cache[n - 1];
}
