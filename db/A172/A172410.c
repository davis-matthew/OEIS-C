/*
 * A172410  Numbers k such that 2k + 9 and 2k + 27 are prime.
 * https://oeis.org/A172410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = the n-th k such that 2k+9 and 2k+27 are prime
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of found k's */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated array capacity */
    static idx_t  tested_k  = 0;    /* largest k that has been tested */

    /* generate until we have at least n terms */
    while (cache_size < n) {
        /* move to next k */
        tested_k += 1;
        seq_t k  = (seq_t)tested_k;                   /* k itself */
        seq_t p1 = k * ((seq_t)2) + ((seq_t)9);       /* 2k+9 */
        seq_t p2 = k * ((seq_t)2) + ((seq_t)27);      /* 2k+27 */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* grow cache array if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 4U;
                grow_seq_array(&cache, &cache_cap, new_cap);
            }
            cache[cache_size++] = k;   /* append new candidate */
        }
    }
    return cache[n - 1];  /* a(1) is the first stored term */
}
