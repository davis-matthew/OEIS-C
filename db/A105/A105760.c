/*
 * A105760  Nonnegative numbers k such that 2k+7 is prime.
 * https://oeis.org/A105760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth nonnegative integer k such that 2k+7 is prime
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of found k's   */
    static idx_t cache_size  = 0;    /* number of entries    */
    static idx_t cache_cap   = 0;    /* allocated capacity   */
    static idx_t next_k = 0;         /* next k to test       */

    /* If we've already computed up to n, just return the result. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache capacity if needed. */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate k's until we have n of them. */
    while (cache_size < n) {
        if (cache_size == cache_cap) {
            cache_cap <<= 1;                /* double capacity   */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Test if 2*next_k + 7 is prime. */
        uint64_t candidate = (uint64_t)next_k * 2ull + 7ull;
        if (is_prime(candidate)) {
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;   /* move to next k for next iteration */
    }

    return cache[n - 1];
}
