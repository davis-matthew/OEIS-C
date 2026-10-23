/*
 * A019404  Primes with primitive root 82.
 * https://oeis.org/A019404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 82 is a primitive root modulo p (OEIS A019404) */
/* The function a(n) returns the n‑th such prime (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Return true iff p is a prime and 82 is a primitive root mod p.  */
/* p is treated as a 64‑bit unsigned integer.                     */
static bool is_primitive_root(uint64_t p)
{
    /* 82 must be coprime to p; if it is a multiple of p, reject immediately. */
    if ((seq_t)82 % (seq_t)p == 0) {
        return false;
    }

    /* φ(p) = p–1.  Factorize φ(p) into its distinct prime divisors. */
    uint64_t phi = p - 1;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(phi, pr, ex, &cnt);

    seq_t base_mod = (seq_t)82 % (seq_t)p;  /* 82 modulo p (always non‑zero here) */
    seq_t mod      = (seq_t)p;

    /* For each prime factor q of φ(p), check g^((p‑1)/q) ≠ 1 (mod p). */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pr[i];
        if (mod_pow(base_mod, phi / q, mod) == (seq_t)1) {
            return false;
        }
    }
    return true;
}

/* Return the n‑th prime p for which 82 is a primitive root modulo p.  */
seq_t a(idx_t n)
{
    static seq_t *results   = NULL;
    static size_t res_len   = 0;
    static size_t res_cap   = 0;
    static uint64_t last_checked = 1;

    /* Lazily allocate storage for results. */
    if (!results) {
        res_cap = 16;
        results = (seq_t *)malloc(res_cap * sizeof(seq_t));
        if (!results) {
            exit(1);
        }
    }

    /* Extend the cache until we have at least n entries. */
    while (res_len < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        if (candidate < 2) candidate = 2;

        /* Find the next prime after last_checked. */
        while (!is_prime((idx_t)candidate)) {
            ++candidate;
        }

        /* Reject p = 2 (and any p that shares a factor with 82). */
        if (candidate != 2 && is_primitive_root(candidate)) {
            /* Grow the array if necessary. */
            if (res_len == res_cap) {
                size_t new_cap = res_cap * 2;
                seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                if (!tmp) {
                    exit(1);
                }
                results = tmp;
                res_cap = new_cap;
            }
            results[res_len++] = (seq_t)candidate;
        }

        last_checked = candidate;
    }

    return results[n - 1];
}
