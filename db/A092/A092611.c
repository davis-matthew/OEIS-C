/*
 * A092611  Product of first n primes that end in 9.
 * https://oeis.org/A092611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n primes that end in 9 (OEIS
 *            A092611)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1; n=0 is undefined – return 0 safely */
    if (n == 0) {
        return 0;
    }

    /* Static state for memoisation */
    static seq_t *prod_cache = NULL;    /* cache of accumulated products */
    static size_t cache_cap   = 0;      /* allocated size of prod_cache    */
    static size_t cache_len   = 0;      /* number of products stored       */
    static seq_t prod          = 1;     /* running product                 */
    static idx_t next_candidate = 19;   /* next number ending in 9 to test   */

    /* Grow cache if needed (double capacity) */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(prod_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return 0 as a safe default */
            return 0;
        }
        prod_cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate primes ending in 9 until we have at least n of them */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            prod *= (seq_t)next_candidate;
            prod_cache[cache_len] = prod;
            cache_len++;
        }
        next_candidate += 10;  /* skip to the next number ending in 9 */
    }

    return prod_cache[n - 1];
}
