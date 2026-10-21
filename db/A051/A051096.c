/*
 * A051096  Primes p such that x^54 = -2 has a solution mod p.
 * https://oeis.org/A051096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that x^54 ≡ -2 (mod p) has a solution. */
/* The sequence starts at a(1)=2.  The function builds primes on demand
   and retains the primes that satisfy the 54‑th power residue condition. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"      /* For seq_t gcd, not used below; local gcd used instead. */
#include "../utils/mod_pow.h"  /* For mod_pow(seq_t base, uint64_t exp, seq_t mod). */

static inline uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void ensure_capacity(size_t *cap, size_t *size, void **ptr, size_t elem_size) {
    if (*size >= *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *ptr = realloc(*ptr, *cap * elem_size);
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n) {
    /* Static state preserved across calls */
    static size_t prime_cap = 0, prime_cnt = 0;
    static idx_t *primes = NULL;

    static size_t res_cap = 0, res_cnt = 0;
    static seq_t *results = NULL;

    static idx_t next_candidate = 2;

    /* Grow result array and compute primes until we have the nth term */
    while (res_cnt < n) {
        idx_t c = next_candidate;
        /* Find the next prime number */
        for (;;) {
            bool isprime = true;
            if (c > 2 && (c & 1) == 0) { ++c; continue; }   /* skip even numbers >2 */

            /* trial division by previously found primes */
            for (size_t i = 0; i < prime_cnt; ++i) {
                idx_t p = primes[i];
                if ((uint64_t)p * (uint64_t)p > c) break;
                if (c % p == 0) { isprime = false; break; }
            }
            if (isprime) break;
            ++c;
        }

        /* Store new prime */
        ensure_capacity(&prime_cap, &prime_cnt, (void **)&primes, sizeof(idx_t));
        primes[prime_cnt++] = c;

        /* Test the 54‑th power residue condition */
        bool ok = false;
        if (c == 2) {
            /* -2 ≡ 0 mod 2, x=0 is a solution */
            ok = true;
        } else {
            uint64_t p_minus_1 = (uint64_t)c - 1;
            uint64_t g = ugcd(p_minus_1, (uint64_t)54);
            uint64_t exp = p_minus_1 / g;
            seq_t base = (seq_t)(c - 2);      /* (-2) mod p == p-2 == c-2 for c>2 */
            seq_t r = mod_pow(base, exp, c);
            if (r == (seq_t)1) ok = true;
        }

        if (ok) {
            ensure_capacity(&res_cap, &res_cnt, (void **)&results, sizeof(seq_t));
            results[res_cnt++] = (seq_t)c;
        }

        next_candidate = c + 1;
    }

    return results[n - 1];
}
