/*
 * A044238  Numbers k such that string 6,3 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has63
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer k such that the octal representation
 * of k contains the consecutive digits 6 followed by 3,
 * but the integer k-1 does not.  The sequence is built incrementally
 * and cached in a static array for repeated calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has63.h"

/* Static cache for computed terms. */
static uint64_t *cache_uint64 = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t next_k = 1;   /* Next integer to examine */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for n=0, return 0 as safeguard */

    while (cache_size < (size_t)n)
    {
        if (cache_cap == 0 || cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            uint64_t *tmp = (uint64_t *)realloc(cache_uint64, new_cap * sizeof(uint64_t));
            if (!tmp)
                return (seq_t)0; /* Allocation failure – sentinel value */
            cache_uint64 = tmp;
            cache_cap = new_cap;
        }

        while (true)
        {
            if (has63(next_k) && !has63(next_k - 1))
            {
                cache_uint64[cache_size++] = next_k;
                next_k++;  /* Advance to next candidate */
                break;
            }
            next_k++;
        }
    }

    return (seq_t)cache_uint64[(size_t)n - 1];
}
