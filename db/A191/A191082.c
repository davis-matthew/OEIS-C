/*
 * A191082  Primes that are not squares mod 83.
 * https://oeis.org/A191082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that (p | 83) = –1 (i.e. p is a
 *            quadratic non‑residue modulo 83)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 963 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
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
    /* static cache to store qualifying primes and keep state between calls */
    static seq_t *cache    = NULL;  /* array of found primes */
    static size_t cached_len = 0;   /* number of primes stored */
    static size_t cached_cap = 0;   /* current allocation capacity */
    static uint64_t next_candidate = 2;  /* next integer to test for primality */

    /* initialise cache on first call */
    if (!cache) {
        cached_cap = 16;
        cache = mem_new(seq_t, cached_cap);
    }

    /* generate primes until we have at least n of them */
    while (cached_len < n) {
        /* advance until next_candidate is a prime */
        while (!is_prime_seq((seq_t)next_candidate))
            ++next_candidate;

        /* check if the prime is a quadratic non‑residue modulo 83 */
        if (jacobi_symbol(next_candidate, 83) == -1) {
            /* grow cache if necessary */
            if (cached_len == cached_cap) {
                cached_cap *= 2;
                cache = mem_resize(seq_t, cache, cached_cap);
            }
            cache[cached_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;  /* move to the next candidate for the next loop */
    }

    return cache[n - 1];
}
