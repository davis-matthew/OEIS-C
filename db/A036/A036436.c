/*
 * A036436  Numbers whose number of divisors is a square.
 * https://oeis.org/A036436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036436: numbers whose number of divisors is a square.
   a(n) = the nth positive integer m such that tau(m) is a perfect square. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) -> seq_t */
#include "../utils/floor_sqrt.h"          /* floor_sqrt(seq_t) -> seq_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t cand = 1;   /* next integer to test */

    /* grow the cache until we have at least n terms */
    while (len < (size_t)n) {
        /* compute number of divisors of cand */
        seq_t tau = divisor_count_u64(cand);

        /* check if tau is a perfect square */
        seq_t r = floor_sqrt(tau);
        if (r * r == tau) {
            /* extend cache if necessary */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failed */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cand;
        }
        cand++;
    }

    return cache[(size_t)n - 1];
}
