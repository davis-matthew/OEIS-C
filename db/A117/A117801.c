/*
 * A117801  Triangular numbers for which the product of the digits is a prime
 *          number.
 * https://oeis.org/A117801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117801: triangular numbers whose digit product is a prime.
   The function searches consecutive triangular numbers, stores the
   ones meeting the criterion in a growing cache, and returns the n‑th
   such number.  It works for any order of calls and uses only ANSI
   C/C++ standard facilities and the utility helpers listed above. */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool */

#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static variable to remember next triangular index to test */
    static idx_t next_k = 1;

    if (n == 0)          /* undefined for n < 1; return 0 as sentinel */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* compute T_next_k = next_k*(next_k+1)/2, using seq_t arithmetic */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;
        next_k++;

        /* product of digits */
        seq_t prod = digit_product(t);
        if (is_prime_big(prod))
        {
            /* grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* out of memory – abort gracefully */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }
    }

    return cache[n - 1];
}
