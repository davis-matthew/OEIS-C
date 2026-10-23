/*
 * A038973  Primes p such that 86 is a square mod p.
 * https://oeis.org/A038973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 86 is a quadratic residue *
 *            modulo p. This includes the primes dividing 86 (p = 2 and 43) *
 *            because 0 ≡ 0² mod p is a square
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* NULL, size_t */

#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/ensure_capacity.h"

/* Cached list of primes p with 86 a quadratic residue mod p.  The list
 * is built lazily; extend_cache will fill it until it contains at
 * least the requested number of elements. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of cached primes */
static size_t cache_cap  = 0;   /* allocated capacity of the array */
static uint64_t next_start = 2; /* next candidate to start searching from */

/* Extend the cache until it has at least 'target' primes. */
static void extend_cache(size_t target)
{
    while (cache_len < target) {
        uint64_t cand = next_start;

        /* Start at 2 if not yet used; otherwise ensure an odd candidate. */
        if (cand <= 2)
            cand = 2;
        else if ((cand & 1ULL) == 0)
            cand++;

        /* Search for the next prime that satisfies the quadratic residue
         * condition for 86.  A prime p divides 86 iff 86 % p == 0, and
         * in that case 86 is clearly a square (0²) modulo p.  For other
         * primes we require the Legendre (Jacobi) symbol (86|p) to be
         * +1. */
        for (;;) {
            if (is_prime((idx_t)cand)) {
                bool ok = (jacobi_symbol(86ULL, cand) == 1) || (86ULL % cand == 0);
                if (ok) {
                    /* Grow the cache if necessary. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        ensure_capacity(cache_len + 1, &cache_cap,
                                        (void **)&cache, sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)cand;
                    /* Next candidate belongs to the next odd number. */
                    next_start = cand + 2;
                    break;
                }
            }
            cand += 2;  /* only odd candidates beyond 2 */
        }
    }
}

/* Return the nth term of the sequence (a(1)=2).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* n = 0 is outside the defined range */
    if (cache_len < (size_t)n)
        extend_cache((size_t)n);
    return cache[(size_t)n - 1];
}
