/*
 * A215280  Primes congruent to {2, 3, 4} mod 13.
 * https://oeis.org/A215280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime congruent to {2, 3, 4} mod 13 (OEIS A215280)  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*  Cache of primes that satisfy the residue condition.
        The cache is filled lazily on demand. */
    static idx_t *primes = NULL;   /* stored primes */
    static size_t  cnt   = 0;      /* number stored */
    static size_t  cap   = 0;      /* array capacity */
    static uint64_t last = 2;      /* next candidate to test */

    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 0 */
    }

    /* Ensure we have at least n primes in the cache. */
    while (cnt < (size_t)n) {
        /* Test prime and residue condition for current candidate. */
        if (((last % 13ULL) == 2ULL || (last % 13ULL) == 3ULL || (last % 13ULL) == 4ULL)
            && is_prime(last)) {
            /* Grow the array if needed. */
            if (cnt == cap) {
                size_t new_cap = (cap ? cap * 2 : 16);
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) {               /* allocation failure – abort. */
                    free(primes);
                    primes = NULL;
                    cnt = cap = 0;
                    return (seq_t)0;
                }
                primes = tmp;
                cap = new_cap;
            }
            primes[cnt++] = last;
        }
        ++last;
    }

    /* Return the requested term. */
    return (seq_t)primes[n - 1];
}
