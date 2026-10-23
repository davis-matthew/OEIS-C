/*
 * A181518  a(n) is the number for which 2^A181516(n)||(2*a(n))!
 * https://oeis.org/A181518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181518
   Numbers m such that Sum_{k>=0} floor(m/2^k) is prime.
   Equivalent to: 2*m - popcount(m) is prime.
   a(1) = 2, a(2) = 4, a(3) = 7, a(4) = 11, ...
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1; if n==0 return 0 as a sentinel */
    if (n == 0) return (seq_t)0;

    /* cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of cached terms */
    static size_t cache_cap   = 0; /* allocated capacity of cache */
    static uint64_t next_candidate = 1; /* next m to test */

    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate++;
        unsigned pc = __builtin_popcountll(cand);

        /* compute 2*cand - pc as seq_t */
        seq_t val = ((seq_t)cand * (seq_t)2) - (seq_t)pc;

        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – fallback to 0 */
                    free(cache);
                    cache = NULL;
                    cache_size = 0;
                    cache_cap   = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
