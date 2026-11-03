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
 * Validated: standard, pass, 77 terms through n = 9516 (2026-11-03)
 * Helpers:   bigomega_seq, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A115024
   Natural numbers n such that the number of prime factors of n
   (counted with multiplicity) equals the number of decimal digits of n.
   The function enumerates values until it reaches the n‑th term. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"


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
        if (bigomega_seq(cand) == (seq_t)num_digits(cand))
        {
            /* grow cache if necessary */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
