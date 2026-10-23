/*
 * A158332  Greater of twin primes, each prime having an even digit sum.
 * https://oeis.org/A158332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 283, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term: the greater prime of a twin prime pair
   whose both primes have even digit sums. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

/* Returns true if the decimal digit sum of x is even. */
static inline bool even_digit_sum(idx_t x)
{
    return (digit_sum((seq_t)x) & 1u) == 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined for n ≥ 1 */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cached = 0;
    static idx_t next_candidate = 3;    /* odd numbers only */

    /* Initialise cache once */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until the requested index is reached */
    while (cached < n) {
        /* Check if next_candidate is part of a twin prime pair */
        if (next_candidate > 2 && is_prime(next_candidate - 2) && is_prime(next_candidate)) {
            if (even_digit_sum(next_candidate) && even_digit_sum(next_candidate - 2)) {
                /* Store the greater prime of the twin pair */
                if (cached == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache,
                                            cache_cap * sizeof(seq_t));
                }
                cache[cached++] = (seq_t)next_candidate;
            }
        }
        /* Advance to the next odd number */
        next_candidate += 2;
    }

    return cache[n - 1];
}
