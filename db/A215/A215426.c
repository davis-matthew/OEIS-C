/*
 * A215426  Primes congruent to {0, 1, 2, 4, 5, 6} mod 19.
 * https://oeis.org/A215426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 0,1,2,4,5,6 (mod 19)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 810 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n = 0, but avoid crash */

    /* static cache of the primes found so far (0‑based index) */
    static seq_t *cache      = NULL;
    static size_t cached    = 0;   /* number of primes stored in cache */
    static size_t cap       = 0;   /* allocated capacity of cache */

    /* initialise cache if this is the first call */
    if (!cache) {
        cap = 16;
        cache = mem_new(seq_t, cap);
    }

    /* extend the cache until it contains at least n primes */
    if (cached < n) {
        idx_t candidate = (cached == 0) ? 2 : (idx_t)cache[cached - 1] + 1;

        while (cached < n) {
            /* find the next prime */
            while (!is_prime(candidate)) ++candidate;

            /* check the required residues modulo 19 */
            idx_t r = candidate % 19;
            if (r == 0 || r == 1 || r == 2 || r == 4 || r == 5 || r == 6) {
                /* grow the cache if necessary */
                if (cached == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (tmp == NULL) {
                        /* allocation failure – abort */
                        abort();
                    }
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[cached++] = (seq_t)candidate;
            }

            ++candidate;
        }
    }

    return cache[n - 1];
}
