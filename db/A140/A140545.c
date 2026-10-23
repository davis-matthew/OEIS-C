/*
 * A140545  Primes of form 17n + 6.
 * https://oeis.org/A140545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n-th prime of the form 17*k + 6 (n≥1)
 * Range:     u8 n <= 3, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_k = 1;  /* next k to test for 17*k+6 */

    if (n == 0)
        return (seq_t)0;  /* undefined for n<1 */

    while (cache_len < n) {
        seq_t candidate = (seq_t)17 * (seq_t)next_k + (seq_t)6;
        if (isPrimeGeneric(candidate)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) /* out of memory, return 0 as failure */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }
    return cache[n - 1];
}
