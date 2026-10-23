/*
 * A040974  Primes p such that x^5 = 15 has a solution mod p.
 * https://oeis.org/A040974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that the congruence x^5 ≡ 15 (mod p) has a
 *            solution
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition is: for p ≠ 3,5, if p ≠ 1 (mod 5) the congruence is solvable; */
/* if p ≡ 1 (mod 5) then it is solvable iff 15^{(p-1)/5} ≡ 1 (mod p). */

#include <stdbool.h>
#include <stddef.h>

/* Uses utility functions provided by the framework */
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Compute the n-th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Last tested integer (next candidate). */
    static uint64_t current = 2;

    /* Ensure the cache can hold at least n terms. */
    while (cache_len < n) {
        /* Find next prime candidate. */
        uint64_t p = current++;
        /* Skip composites by testing primality. */
        while (!is_prime(p))
            p = current++;

        /* Determine if p belongs to the sequence. */
        bool include = false;
        if (p == 2 || p == 3 || p == 5) {
            include = true;                       /* p divides 15 → trivial solution */
        } else if (p % 5 != 1) {
            include = true;                       /* 5 does not divide p-1 → always solvable */
        } else {
            /* p ≡ 1 (mod 5) → need to check exponent. */
            uint64_t exp = (p - 1) / 5;           /* exponent fits in 64‑bit */
            seq_t r = mod_pow(15, exp, p);        /* compute 15^{exp} mod p */
            include = (r == 1);
        }

        /* Append to cache if included. */
        if (include) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;              /* allocation failure: undefined behavior */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
