/*
 * A040140  Primes p such that x^4 = 17 has no solution mod p.
 * https://oeis.org/A040140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A040140 - primes p such that x^4 = 17 has no solution mod p
 *            Computed by generating primes and testing the quartic residue
 *            condition
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 913 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Static cache of qualifying primes */
    static seq_t *prime_cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 3; /* first odd prime > 2 */

    /* Grow cache as needed */
    while (cache_len < n) {
        idx_t cand = next_candidate;
        next_candidate += 2;  /* next odd number */

        if (!is_prime(cand))
            continue;

        /* Prime 17 yields x=0 as a solution, so skip it. */
        if (cand == 17)
            continue;

        int has_solution = 0;
        if ((cand & 3) == 1) {   /* cand % 4 == 1 */
            idx_t exp = (cand - 1) / 4;
            seq_t r = powmod((seq_t)17, exp, (seq_t)cand);
            has_solution = (r == (seq_t)1);
        } else {                /* cand % 4 == 3 */
            /* Jacobi symbol (17/p) == 1 iff 17 is a quadratic residue.
               For p % 4 == 3, a 4th power exists iff it's a quadratic residue. */
            has_solution = (jacobi_symbol(17, cand) == 1);
        }

        if (!has_solution) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                if (!tmp) return (seq_t)0; /* allocation failure */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)cand;
        }
    }

    return prime_cache[n - 1];
}
