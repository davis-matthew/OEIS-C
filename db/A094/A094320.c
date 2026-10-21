/*
 * A094320  Numbers n >= 0 such that Lehmer's polynomial 263*n^2+3 is prime.
 * https://oeis.org/A094320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 315, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 263*n^2 + 3 is prime. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static seq_t next_candidate = 0;

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i) {
            while (1) {
                seq_t val = (seq_t)263 * next_candidate * next_candidate + (seq_t)3;
                if (isPrimeGeneric(val))
                    break;
                ++next_candidate;
            }
            cache[i] = next_candidate;
            ++next_candidate;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
