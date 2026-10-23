/*
 * A115024  Natural numbers n such that the number of prime factors of n
 *          (counted with multiplicity) is equal to the number of decimal
 *          digits of n.
 * https://oeis.org/A115024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 8010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A115024
   Natural numbers n such that the number of prime factors of n
   (counted with multiplicity) equals the number of decimal digits of n.
   The function enumerates values until it reaches the n‑th term. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_prime_factors.h"   /* seq_t count_prime_factors(seq_t) */
#include "../utils/digit_len.h"            /* int digit_len(seq_t) */

#if defined(__cplusplus)
#   define REALLOC(type, ptr, size) reinterpret_cast<type>(realloc((ptr), (size)))
#else
#   define REALLOC(type, ptr, size) ((type)realloc((ptr), (size)))
#endif

seq_t a(idx_t n)
{
    /* cache of found terms, 1‑based indexing: cache[0] = a(1) */
    static seq_t *cache = NULL;          /* pointer to dynamic array */
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_cap  = 0;        /* current capacity of cache */
    static seq_t cand = (seq_t)2;        /* next integer to test */

    /* cache must contain at least n terms */
    while (cache_size < (size_t)n)
    {
        /* test candidate */
        if (count_prime_factors(cand) == (seq_t)digit_len(cand))
        {
            /* grow cache if necessary */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = REALLOC(seq_t *, cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        ++cand;                                 /* next integer */
    }

    return cache[n - 1];                        /* 1‑based indexing */
}
