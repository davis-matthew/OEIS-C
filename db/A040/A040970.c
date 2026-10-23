/*
 * A040970  Primes p such that x^5 = 13 has a solution mod p.
 * https://oeis.org/A040970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that x^5 = 13 has a solution modulo p.
   Criterion:
     - If p != 13 and p % 5 != 1, the congruence always has a solution
       (since gcd(5, p-1) = 1).
     - If p % 5 == 1, we need 13^((p-1)/5) ≡ 1 (mod p).
   The function builds a static cache of the accepted primes up to the
   requested index.  For primality testing it uses is_prime, and for the
   5‑th power test it uses mod_pow.  The cache grows as needed.  The
   algorithm is O(n) for the n‑th term.  */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Handle trivial request */
    if (n == 0) return 0;

    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;   /* last integer tested */

    /* Grow cache if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_checked = 1;
    }

    for ( ; cache_len < n; ++last_checked ) {
        /* Skip even numbers >2 */
        if (last_checked > 2 && (last_checked & 1) == 0) continue;

        /* Test primality */
        if (!is_prime(last_checked)) continue;

        /* Test congruence condition */
        if (last_checked == 13) {
            /* 13 divides the RHS, always solvable (x≡0) */
        } else if (last_checked % 5 != 1) {
            /* gcd(5, p-1) == 1 ⇒ always solvable */
        } else {
            /* Need 13^((p-1)/5) ≡ 1 mod p */
            idx_t exp = (last_checked - 1) / 5;
            seq_t mod = (seq_t)last_checked;
            if (mod_pow((seq_t)13, (uint64_t)exp, mod) != (seq_t)1)
                continue;   /* not a solution */
        }

        /* Accept this prime */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
