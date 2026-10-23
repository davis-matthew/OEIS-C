/*
 * A024904  Numbers k such that 7*k - 5 is prime.
 * https://oeis.org/A024904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 804, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024904: numbers k such that 7*k - 5 is prime.
   The function returns the n‑th such number (1‑based index). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>                 /* malloc, realloc */

#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;
    static idx_t  cap   = 0;
    static idx_t  last_k = 0;        /* largest k examined so far */

    /* Extend cache until it contains at least n terms */
    for (idx_t k = last_k + 1; len < n; ++k)
    {
        seq_t p = (seq_t)7 * k - (seq_t)5;   /* compute 7*k-5 */

        if (is_prime_big(p))
        {
            /* grow cache if needed */
            if (len == cap)
            {
                idx_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *new_arr = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                if (!new_arr) abort();   /* allocation failed */
                cache = new_arr;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)k;
        }
        last_k = k;
    }

    return cache[n - 1];
}
