/*
 * A143829  Numbers n such that 10n^2 - 1 is prime.
 * https://oeis.org/A143829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143829: Numbers n such that 10*n^2 - 1 is prime.
   Computed by searching for n in increasing order and testing 10*n*n - 1
   for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        last_k++;
        seq_t val = (seq_t)10 * (seq_t)last_k * (seq_t)last_k - 1;
        if (isPrimeGeneric(val)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_k;
        }
    }

    return cache[n - 1];
}
