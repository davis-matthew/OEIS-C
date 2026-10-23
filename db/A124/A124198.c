/*
 * A124198  Numbers k such that 21*k + 1 is prime.
 * https://oeis.org/A124198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k such that 21*k + 1 is prime
 * Range:     u8 n <= 3, u16 n <= 568, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1; offset is 1 */

    /* static cache of discovered terms */
    static seq_t *cache = NULL;          /* array of found k-values */
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_capacity = 0;    /* allocated array size */
    static idx_t next_candidate_k = 1;   /* next k to test */

    /* increase cache capacity if needed */
    if (cache_size < (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* generate further terms until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)21 * (seq_t)next_candidate_k + 1;
        if (isPrimeGeneric(cand)) {
            cache[cache_size++] = (seq_t)next_candidate_k;
        }
        next_candidate_k++;
    }

    return cache[n - 1];
}
