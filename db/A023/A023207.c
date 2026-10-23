/*
 * A023207  Numbers m such that m and 2*m + 9 both prime.
 * https://oeis.org/A023207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers m such that m and 2*m + 9 are both prime
   This implementation builds each term on demand and caches
   previously found values.  It uses the provided helper
   bool is_prime(idx_t) from "../utils/is_prime.h". */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 2;

    /* Grow the cache until it contains at least n terms.  */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 128;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*tmp));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find additional terms up to n.  */
        while (cache_len < (size_t)n) {
            for (;;) {
                idx_t m = next_candidate++;
                if (is_prime(m) &&
                    is_prime((idx_t)(2 * m + 9))) {
                    cache[cache_len++] = (seq_t)m;
                    break;
                }
            }
        }
    }
    return cache[(size_t)n - 1];
}
