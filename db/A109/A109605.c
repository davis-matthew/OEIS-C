/*
 * A109605  Numbers n such that 100000n + 91111 is prime.
 * https://oeis.org/A109605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A109605:  Numbers n such that 100000*n + 91111 is prime.
 * The function returns the nth such number, with a(1)=6, a(2)=7,...
 */

#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;
    static idx_t next_candidate = 1;   /* next n to test */

    if (n == 0) return 0;              /* undefined for offset 0 */

    /* grow cache until we have at least n terms */
    while (cache_size < n) {
        uint64_t value = (uint64_t)100000 * (uint64_t)next_candidate + 91111;
        if (is_prime_uint64(value)) {
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
