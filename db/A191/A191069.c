/*
 * A191069  Primes p that have Kronecker symbol (p|38) = -1.
 * https://oeis.org/A191069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the Kronecker symbol (p|38) =
 *            –1. (p|38) factors as (p|2)*(p|19); the Legendre symbols are
 *            computed via parity and quadratic residues modulo 19
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   legendre_p19, legendre_p2, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/legendre_p19.h"
#include "../utils/legendre_p2.h"

/* Good prime test: (p|38) = –1. */
static inline bool good_prime(uint64_t p)
{
    return legendre_p2(p) * legendre_p19(p) == -1;
}

/* Return the nth element of A191069.  Indexing is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n ≤ 0, return 0 safely */
        return 0;

    /* Static cache of primes satisfying the condition. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;
    static uint64_t last_prime = 2;   /* prime to start scanning after */

    /* Grow cache until it contains at least n values. */
    while (len < n) {
        last_prime = next_prime(last_prime + 1);
        if (!good_prime(last_prime))
            continue;

        /* Resize if necessary. */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache)
                return 0;   /* allocation failure → error value */
            if (cache)
                memcpy(new_cache, cache, len * sizeof(seq_t));
            free(cache);
            cache   = new_cache;
            cap     = new_cap;
        }
        cache[len++] = (seq_t)last_prime;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
