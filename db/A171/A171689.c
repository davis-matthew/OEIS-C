/*
 * A171689  Nonprimes n such that either n+-1 is prime.
 * https://oeis.org/A171689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171689: Non‑prime integers n such that exactly one of n‑1 or n+1 is prime
 * (index offset 1).  The function incrementally searches integers,
 * memoises the found terms, and returns the nth term.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_int = 1;   /* next integer to test */

    /* The sequence is defined for n ≥ 1.  Return 0 for n == 0 (never used). */
    if (n == 0) return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Only non‑prime numbers qualify. */
        if (!is_prime(next_int))
        {
            bool left  = is_prime(next_int - 1);
            bool right = is_prime(next_int + 1);
            if (left ^ right)   /* exactly one neighbour is prime */
            {
                /* Ensure enough space in the cache. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_int;
            }
        }
        ++next_int;
    }

    return cache[n - 1];
}
