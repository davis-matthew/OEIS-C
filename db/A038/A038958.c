/*
 * A038958  Primes p such that 73 is not a square mod p.
 * https://oeis.org/A038958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth prime p for which 73 is not a quadratic residue
   modulo p (i.e., the Legendre symbol (73|p) = -1).  The function builds
   and caches the qualifying primes on demand, allowing calls in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0; return a sentinel */

    /* Static cache of qualifying primes and tracking state */
    static seq_t *cache = NULL;       /* array of primes */
    static size_t cache_len = 0;      /* number of stored primes */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t candidate = 5;       /* next odd integer to test */

    /* Grow the cache until we have at least n primes */
    while (cache_len < (size_t)n) {
        /* Skip non‑primes */
        while (!is_prime(candidate)) {
            candidate += 2;           /* only odds after 3 */
        }

        /* Accept if 73 is a quadratic non‑residue modulo candidate */
        if (jacobi_symbol(73ULL, (uint64_t)candidate) == -1) {
            /* Ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;    /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        /* Advance to the next odd candidate for the next iteration */
        candidate += 2;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
