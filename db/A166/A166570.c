/*
 * A166570  Numbers n such that 12*n+11 is not prime.
 * https://oeis.org/A166570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of n such that 12*n+11 is not prime (OEIS A166570) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static idx_t next_candidate = 1;
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms */
    while (cache_size < n)
    {
        /* Allocate or grow cache if needed */
        if (cache_capacity == 0)
        {
            cache_capacity = 16;
            cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        }
        else if (cache_size == cache_capacity)
        {
            cache_capacity *= 2;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        }

        /* Test whether 12*next_candidate+11 is composite */
        idx_t cand = next_candidate * 12 + 11;
        bool composite = !is_prime(cand);

        if (composite)
        {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
