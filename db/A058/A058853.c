/*
 * A058853  Primes p such that x^43 = 2 has a solution mod p.
 * https://oeis.org/A058853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which the congruence x^43 ≡ 2 (mod
 *            p) has a solution. The sequence consists of primes p such that
 *            either 43 ∤ (p‑1), or 43 | (p‑1) and 2^((p‑1)/43) ≡ 1 (mod p)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"
#include "../utils/seq_realloc.h"

static inline bool has_solution(uint64_t p)
{
    if ((p - 1) % 43 != 0)        /* 43 does not divide p‑1        */
        return true;              /* then the congruence is solvable */
    uint64_t e = (p - 1) / 43;    /* raise 2 to (p‑1)/43 mod p      */
    return powmod64(2, e, p) == 1;
}

seq_t a(idx_t n)
{
    /* static cache of primes satisfying the property */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* next integer to test for primality (starts at 2) */
    static uint64_t candidate = 2;

    /* iterate until we have the n-th term */
    while (cache_len < (size_t)n) {
        /* find the next prime */
        while (!is_prime(candidate)) {
            candidate = (candidate == 2) ? 3 : candidate + 2;
        }

        /* candidate is prime; check the required condition */
        if (has_solution(candidate)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        /* advance to the next candidate number */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return cache[(size_t)n - 1];
}
