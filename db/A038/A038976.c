/*
 * A038976  Primes p such that 87 is not a square mod p.
 * https://oeis.org/A038976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A038976: Primes p for which 87 is a quadratic non‑residue modulo p.
   Implementation: generate primes in order, test the Legendre symbol
   (87|p) using Euler’s criterion.  Store the primes that satisfy
   (87|p) = –1.  The cache allows out‑of‑order requests.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return the n-th prime p such that 87 is not a quadratic residue mod p.
   n >= 1 maps to the first element (5). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached primes */
    static size_t cache_size = 0;    /* allocated cache size */
    static size_t count = 0;         /* number of cached primes */
    static uint64_t next_candidate = 3; /* next integer to test for primality */

    if (n == 0) return 0;            /* safety for offset 1 */

    /* Extend cache until it contains at least n elements */
    while (count < (size_t)n) {
        /* Find the next prime ≥ next_candidate */
        while (!is_prime((idx_t)next_candidate)) {
            ++next_candidate;
        }
        uint64_t p = next_candidate;
        ++next_candidate;           /* prepare for next search */

        /* Euler's criterion: 87^((p-1)/2) mod p ≡ –1 ⇔ 87 a quadratic non‑residue */
        uint64_t exp = (p - 1) >> 1;          /* (p-1)/2 */
        seq_t r      = mod_pow((seq_t)87, exp, (seq_t)p);
        seq_t target = (seq_t)p - 1;          /* –1 mod p */

        if (r == target) {
            /* Grow cache if necessary */
            if (count == cache_size) {
                size_t new_size = cache_size ? cache_size * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation error: return 0 as placeholder */
                cache = tmp;
                cache_size = new_size;
            }
            cache[count++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
