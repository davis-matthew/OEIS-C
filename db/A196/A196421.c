/*
 * A196421  a(n) = prime(n)*T(n), where T = A000217.
 * https://oeis.org/A196421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * T(n), T(n) = n*(n+1)/2 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1025, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   free_seq_array, get_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* Compute the nth term of OEIS A196421:   a(n) = prime(n) * n*(n+1)/2   */
/* Indexing follows the OEIS offset: a(1) is the first term.          */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety (offset depends on implementation). */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation storage for previously computed values. */
    static seq_t *cache   = NULL;   /* array indexed from 0..cap-1 */
    static idx_t   cap    = 0;      /* allocated capacity (number of slots) */
    static idx_t   last   = 0;      /* largest index computed so far */

    /* Grow the array if needed so that index n is addressable. */
    if (n >= cap) {
        idx_t new_cap = n + 1;            /* need indices 0..n inclusive */
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (!new_cache)
            abort();                     /* allocation failure */

        /* Copy old values if any. */
        if (cache) {
            for (idx_t i = 0; i <= last; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }

        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing entries up to n. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t p   = (seq_t)get_prime(i);              /* prime(i) */
            seq_t tri = (seq_t)i * ((seq_t)i + 1) / 2;    /* triangular number T(i) */
            cache[i]  = p * tri;                          /* product */
        }
        last = n;
    }

    return cache[n];
}
