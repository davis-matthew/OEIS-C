/*
 * A151926  Numbers n such that A151925(n) = 3.
 * https://oeis.org/A151926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers n such that n ∉ A008784 and n ≠ 0,4,7 (mod 8)
 *            (i.e. A151926 = N \ {A008784, numbers == 0,4,7 mod 8})
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   in_A008784
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/in_A008784.h"

static seq_t *cache = NULL;          /* stored terms of the sequence */
static size_t cache_used = 0;        /* how many terms are stored */
static size_t cache_cap   = 0;        /* current allocated capacity */
static idx_t   next_candidate = 1;   /* next natural number to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe */

    /* Grow the cache until we have at least n terms */
    while (cache_used < n) {
        ++next_candidate;
        /* Exclude numbers congruent to 0,4,7 (mod 8) */
        idx_t mod8 = next_candidate & 7;   /* faster than %8 */
        if (mod8 == 0 || mod8 == 4 || mod8 == 7)
            continue;

        /* Exclude numbers in A008784 */
        if (in_A008784(next_candidate))
            continue;

        /* Store the accepted number */
        if (cache_used >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – abort */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_used++] = (seq_t)next_candidate;
    }

    return cache[n - 1];
}
