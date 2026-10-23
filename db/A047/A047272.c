/*
 * A047272  Primes p such that p == 1 (mod 4) and Fibonacci((p-1)/4) is
 *          divisible by p.
 * https://oeis.org/A047272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 771, u32 n <= 772, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A047272: primes p ≡ 1 (mod 4) such that Fibonacci((p-1)/4) is
   divisible by p.  The sequence is generated on demand; elements are
   cached so that calls with arbitrary n order are efficient. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Helper: fast doubling Fibonacci modulo mod using only seq_t arithmetic. */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t a = 0, b = 1;
    for (int i = 63; i >= 0; --i) {
        /* F(2k)  = F(k)·(2·F(k+1) − F(k)) */
        seq_t two_b = (b * 2) % mod;
        seq_t t     = (two_b + mod - a) % mod;
        seq_t d     = (a * t) % mod;

        /* F(2k+1)=F(k)² + F(k+1)² */
        seq_t aa = (a * a) % mod;
        seq_t bb = (b * b) % mod;
        seq_t e  = (aa + bb) % mod;

        if ((n >> i) & 1ULL) {  /* odd */
            a = e;
            b = (d + e) % mod;
        } else {                /* even */
            a = d;
            b = e;
        }
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t size    = 0;
    static size_t capacity = 0;
    static uint64_t last_candidate = 3;  /* start searching from 5 */

    /* Extend cache until it contains n terms. */
    while (size < (size_t)n) {
        /* Find next prime p ≡ 1 (mod 4) satisfying the Fibonacci condition. */
        uint64_t p;
        for (;;) {
            /* Move to the next odd candidate. */
            if (last_candidate <= 3) last_candidate = 5;
            else if ((last_candidate & 1ULL) == 0) ++last_candidate;

            /* Primality test. */
            if (!is_prime(last_candidate)) {
                last_candidate += 2;
                continue;
            }

            /* Check the mod 4 condition. */
            if ((last_candidate & 3ULL) != 1) {
                last_candidate += 2;
                continue;
            }

            /* Fibonacci condition. */
            idx_t k = (last_candidate - 1) / 4;
            if (fib_mod(k, (seq_t)last_candidate) == 0) {
                p = last_candidate;
                break;          /* found suitable prime */
            }
            last_candidate += 2;
        }

        /* Store the new term. */
        if (size == capacity) {
            size_t new_cap = capacity ? capacity * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failed */
            cache = tmp;
            capacity = new_cap;
        }
        cache[size++] = (seq_t)p;

        /* Advance candidate past the prime just found. */
        last_candidate = p + 2;
    }

    /* Return the n‑th term (1‑based). */
    return cache[(size_t)n - 1];
}
