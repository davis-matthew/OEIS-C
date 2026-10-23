/*
 * A154253  Primes of the form 9n^2-8n+2.
 * https://oeis.org/A154253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 9*n^2-8*n+2 (with n starting at 0)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 17, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2, a(2) = 3, a(3) = 59, ...  as listed in OEIS A154253. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms (1‑based index). */
    static seq_t *cache   = NULL;    /* array of primes */
    static size_t  sz     = 0;       /* number of stored terms */
    static size_t  cap    = 0;       /* capacity of the array */
    static idx_t   m      = 0;       /* next n to evaluate (0‑based) */

    /* Generate terms until we have at least n of them. */
    while (sz < (size_t)n) {
        seq_t k = (seq_t)m;
        seq_t v = (seq_t)9 * k * k - (seq_t)8 * k + (seq_t)2;

        if (is_prime_big(v)) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = v;
        }
        ++m;
    }

    return cache[n - 1];
}
