/*
 * A216067  Prime numbers p such that p is odd and is congruent to 2 (mod 5)
 *          or 3 (mod 5), but the period of the irreducible polynomial x^2-x-1
 *          in GF(p^2) is not 2*(p+1).
 * https://oeis.org/A216067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 692, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216067:
 *  Prime numbers p such that
 *      p is odd,
 *      p ≡ 2 (mod 5) or p ≡ 3 (mod 5),
 *      and the period of x²–x–1 over GF(p²) (i.e. the Pisano period of
 *      Fibonacci numbers modulo p) is NOT equal to 2·(p+1).
 *
 *  The function below computes the n‑th such prime by searching primes
 *  sequentially and testing the period condition.  Results are cached
 *  so repeated calls for the same or larger n are cheap.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility from the build system – gives the next prime ≥ start. */
#include "../utils/next_prime.h"

/* ------------------------------------------------------------ */
/* Fibonacci modulo p – fast doubling                           */
/* ------------------------------------------------------------ */
static void fib_pair_mod(uint64_t p, uint64_t n, uint64_t *fk, uint64_t *fk1)
{
    if (n == 0) {
        *fk  = 0;
        *fk1 = 1 % p;
        return;
    }

    uint64_t a, b;
    fib_pair_mod(p, n >> 1, &a, &b);

    /* F_{2k} = F_k * (2*F_{k+1} – F_k) */
    uint64_t t = (2 * b) % p;
    t = (t + p - a) % p;
    uint64_t d = (uint64_t)((__uint128_t)a * t % p);

    /* F_{2k+1} = F_k² + F_{k+1}² */
    uint64_t e = (uint64_t)(((__uint128_t)a * a) % p +
                          ((__uint128_t)b * b) % p) % p;

    if ((n & 1) == 0) {
        *fk  = d;
        *fk1 = e;
    } else {
        *fk  = e;
        *fk1 = (d + e) % p;
    }
}

/* ------------------------------------------------------------ */
/* Check whether the Pisano period of Fibonacci numbers modulo
 *  prime p equals the maximal value 2·(p+1).                       */
/* ------------------------------------------------------------ */
static bool period_is_full(uint64_t p)
{
    uint64_t M = 2 * (p + 1);      /* maximal divisor */
    uint64_t k = M;
    uint64_t t = M;

    /* trial division of M */
    for (uint64_t q = 2; q * q <= t; ++q) {
        if (t % q != 0) continue;
        int cnt = 0;
        while (t % q == 0) { t /= q; ++cnt; }

        for (int i = 0; i < cnt; ++i) {
            while (k % q == 0) {
                uint64_t trial = k / q;
                uint64_t fk, fk1;
                fib_pair_mod(p, trial, &fk, &fk1);
                if (fk == 0 && fk1 % p == 1) {
                    k = trial;
                } else {
                    break;
                }
            }
        }
    }

    if (t > 1) { /* remaining prime factor */
        uint64_t q = t;
        while (k % q == 0) {
            uint64_t trial = k / q;
            uint64_t fk, fk1;
            fib_pair_mod(p, trial, &fk, &fk1);
            if (fk == 0 && fk1 % p == 1) {
                k = trial;
            } else {
                break;
            }
        }
    }

    return k == M;
}

/* ------------------------------------------------------------ */
/* Cached sequence storage                                       */
/* ------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t last_tested_prime = 1;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, just return 0 */

    if (seq_len >= (size_t)n) return seq_cache[n - 1];

    /* Build the sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        /* Find next prime. */
        uint64_t p = next_prime(last_tested_prime + 1);
        last_tested_prime = p;

        if (p == 2) continue;                 /* skip even prime */

        uint64_t mod5 = p % 5;
        if (mod5 != 2 && mod5 != 3) continue; /* only 2 or 3 mod 5 */

        if (!period_is_full(p)) {
            /* Accept this prime. */
            ensure_cache_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)p;
        }
    }

    return seq_cache[n - 1];
}
