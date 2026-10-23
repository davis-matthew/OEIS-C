/*
 * A038932  Primes p such that 53 is not a square mod p.
 * https://oeis.org/A038932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate primes and include those for which 53 is a
   quadratic non‑residue.  The sequence starts with 2, then all
   odd primes p≠53 with (53|p) = –1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

/* True iff 53 is a quadratic non‑residue modulo the odd prime p
   (for p = 53 it returns false because 53 ≡ 0 (mod 53)). */
static bool is_non_square_mod_53(uint64_t p)
{
    /* Euler’s criterion: (53|p) ≡ 53^((p-1)/2) (mod p).
       It equals p‑1 iff the Legendre symbol is –1. */
    return powmod64(53, (p - 1) / 2, p) == p - 1;
}

/* Cached sequence values and generation state */
static seq_t *cache              = NULL;   /* array of primes satisfying the condition */
static idx_t  cache_size         = 0;      /* number of stored terms          */
static idx_t  cache_cap          = 0;      /* allocated capacity for cache    */
static uint64_t next_candidate   = 2;      /* next integer to test for primality */

/* Grow the cache array to hold at least 'need' elements. */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap = new_cap * 2;

    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)new_cap * sizeof(seq_t));
    /* realloc failure is unlikely for the required sizes */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)                      /* behaviour below the offset is unspecified */
        return 0;

    /* Generate terms until we have at least 'n' of them. */
    while (cache_size < n) {
        /* Test the current candidate for primality. */
        if (is_prime(next_candidate)) {
            /* Include the prime if it satisfies the non‑square condition.
               2 is always included by convention. */
            if (next_candidate == 2 ||
                is_non_square_mod_53(next_candidate)) {

                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }

        /* Advance to the next candidate. After 2 the candidates are odd. */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
