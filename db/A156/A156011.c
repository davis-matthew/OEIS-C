/*
 * A156011  Primes p such that 2*p+3 and 16*p+45 are both prime.
 * https://oeis.org/A156011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 526, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that 2*p+3 and 16*p+45 are both prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static idx_t *cache = NULL;
    static size_t cap = 0, used = 0;

    if (n == 0) return 0; /* undefined, but keep harmless */

    /* Generate terms until we reach the requested index */
    if (used < n) {
        /* Start from the first prime to test */
        idx_t cand = (used == 0) ? 2 : cache[used - 1] + 1;

        for (;; cand++) {
            /* First, candidate must be prime itself */
            if (!is_prime(cand))
                continue;

            /* Check the two derived numbers */
            if (!is_prime(cand * 2ULL + 3ULL))
                continue;
            if (!is_prime(cand * 16ULL + 45ULL))
                continue;

            /* Store the found term */
            if (used >= cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!tmp) { /* allocation failure – bail out */
                    cache = NULL; cap = used = 0;
                    return 0;
                }
                cache = tmp;
                cap = new_cap;
            }
            cache[used++] = cand;

            if (used >= n)
                break;
        }
    }

    return (seq_t)cache[n - 1];
}
