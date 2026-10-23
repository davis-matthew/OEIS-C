/*
 * A191346  Primes = 1 mod 17#.
 * https://oeis.org/A191346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 1 modulo 17# (510510)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Returns the n-th term of OEIS A191346.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found primes */
    static size_t cap   = 0;      /* current capacity of cache   */
    static size_t sz    = 0;      /* number of primes stored     */
    static idx_t  next_k = 1;     /* k for the next candidate (510510*k + 1) */

    /* Magic modulus: 17# = 2·3·5·7·11·13·17 = 510510 */
    static const seq_t MOD  = 510510;
    static const seq_t OFF  = 1;

    /* Return cached value if already computed. */
    if (n <= sz)
        return cache[n - 1];

    /* Generate new candidates until we have at least n primes. */
    while (sz < (size_t)n) {
        seq_t cand = MOD * (seq_t)next_k + OFF;  /* candidate = 510510*k + 1 */
        ++next_k;

        if (is_prime_big(cand)) {
            /* Grow the cache array if needed. */
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = cand;              /* store the found prime */
        }
    }

    return cache[n - 1];
}
