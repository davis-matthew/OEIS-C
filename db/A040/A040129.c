/*
 * A040129  Primes p such that x^4 = 14 has no solution mod p.
 * https://oeis.org/A040129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that the congruence x^4 ≡ 14 (mod p)
 *            has no solution
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

/* Test whether the congruence x^4 ≡ 14 (mod p) has a solution. */
static inline bool has_solution(uint64_t p)
{
    /* The congruence is solvable for p = 2 or p = 7. */
    if (p == 2 || p == 7)
        return true;

    /* For odd primes p, use the theory of fourth‑powers. */
    if ((p & 3ULL) == 3ULL) {                /* p ≡ 3 (mod 4) */
        /* Then 14 must be a quadratic residue modulo p for a solution to exist. */
        return jacobi_symbol(14ULL, p) == 1;
    }

    /* For p ≡ 1 (mod 4) we need 14^((p‑1)/4) ≡ 1 (mod p). */
    return mod_pow((seq_t)14, (p - 1ULL) / 4ULL, (seq_t)p) == (seq_t)1;
}

/* --------------------------------------------------------------------- */
/* The sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state, shared among all calls. */
    enum { MAX_CACHE = 20000 };

    static uint64_t last_checked = 2ULL;      /* last prime considered   */
    static seq_t   cache[MAX_CACHE];
    static size_t  cache_len = 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime after last_checked. */
        uint64_t candidate = last_checked + 1ULL;
        if (candidate <= 2ULL)
            candidate = 3ULL;                 /* first odd candidate */

        /* Skip even numbers. */
        if ((candidate & 1ULL) == 0ULL)
            candidate++;

        /* Search for the next prime. */
        while (!is_prime(candidate))
            candidate += 2ULL;                /* only odd candidates */

        /* candidate is prime. Store it if it satisfies the condition. */
        if (!has_solution(candidate)) {
            if (cache_len < (size_t)MAX_CACHE)
                cache[cache_len++] = (seq_t)candidate;
        }

        last_checked = candidate;            /* next search starts after this prime */
    }

    return cache[(size_t)n - 1];
}
