/*
 * A086392  Numbers k such that k/2 is composite and F(k^2+1)==5 (mod k) where
 *          F(k) denotes the k-th Fibonacci number.
 * https://oeis.org/A086392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 202, u32 n <= 202, u64 n <= 202,
 *            u128 n <= 202, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the n‑th integer k ≥ 8 such that
   – k is even,
   – k/2 is composite,
   – Fib(k²+1) ≡ 5 (mod k).
   This function computes the nth such k by searching progressively
   and caching found values. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* fast‑doubling Fibonacci modulo mod,  n is 64‑bit index */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t a = 0;                     /* F(0)  mod mod */
    seq_t b = 1;                     /* F(1)  mod mod */
    if (mod == 0) return 0;          /* never happens for k ≥ 8 */

    for (int i = 63; i >= 0; i--) {
        /* compute (2*b – a) mod mod without overflow */
        seq_t bb = (b + b) % mod;                                 /* 2*b mod mod */
        seq_t t  = (bb >= a) ? (bb - a) : (bb + mod - a);          /* (2*b – a) mod mod */

        seq_t c = (a * t) % mod;                                 /* F(2*i)   */
        seq_t d = ((a * a) % mod + (b * b) % mod) % mod;         /* F(2*i+1) */

        if ((n >> i) & 1) {                                     /* use odd case */
            a = d;
            b = (c + d) % mod;
        } else {                                                 /* even case */
            a = c;
            b = d;
        }
    }
    return a;
}

/* ----- cache for the found terms ----- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t next_k = 8;   /* first candidate k */

/* public entry point ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* ==== candidate k ==== */
        /* next_k always even, start at 8 */
        idx_t k = next_k;

        /* ---- test k --------------------------------------------------- */
        idx_t half = k >> 1;                  /* k/2 */

        /* half must be composite: not prime and > 1 (>=4 here) */
        if (!is_prime(half)) {
            /* compute Fib(k²+1) modulo k */
            idx_t idx = k * k + 1;         /* k² + 1, fits in uint64_t for needed n */
            seq_t f = fib_mod(idx, k);

            if ((f % k) == (seq_t)5 % k) {   /* condition F(k²+1) ≡ 5 (mod k) */
                /* store the term */
                if (cache_size >= cache_cap) {
                    cache_cap = (cache_cap ? cache_cap * 2 : 16);
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) abort();      /* out of memory */
                    cache = tmp;
                }
                cache[cache_size++] = k;
            }
        }

        next_k += 2;   /* next even integer */
    }

    return cache[(size_t)n - 1];
}
