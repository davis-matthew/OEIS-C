/*
 * A030521  Product of first n palindromic primes plus 1.
 * https://oeis.org/A030521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (product of the first n palindromic primes) + 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   make_palindrome_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the definition, accumulating palindromic
   primes on-demand and caching intermediate products. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/make_palindrome_odd.h"

/* ---- Helper: 64‑bit deterministic Miller‑Rabin primality test ---- */

static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    /* Uses __int128 for portability under C++17 and C11 that support it. */
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mulmod_uint64(result, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller–Rabin with bases sufficient for 64‑bit integers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Deterministic bases for n < 2^64 */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----- Main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Static cache of cumulative products:
       prod[i] = product of first i palindromic primes.
       prod[0] = 1. */
    static seq_t *prod = NULL;
    static size_t prod_cap = 0;
    static size_t prod_size = 0;          /* number of stored products (>=1) */

    /* Static state for palindrome generation */
    static uint64_t next_prefix = 10;      /* next prefix for 3‑digit palindromes */

    /* Initialise on first call */
    if (prod == NULL) {
        prod_cap = 16;
        prod = (seq_t *)malloc(prod_cap * sizeof(seq_t));
        if (!prod) return (seq_t)0;       /* allocation failure */

        prod[0] = (seq_t)1;              /* product of zero primes */

        /* Pre‑populate the first five palindromic primes:
            2, 3, 5, 7, 11  */
        prod[1] = (seq_t)2;              /* 2 */
        prod[2] = (seq_t)6;              /* 2·3 */
        prod[3] = (seq_t)30;             /* 2·3·5 */
        prod[4] = (seq_t)210;            /* 2·3·5·7 */
        prod[5] = (seq_t)2310;           /* 2·3·5·7·11 */
        prod_size = 6;                   /* indices 0..5 exist */

        next_prefix = 10;                /* start after 11 (3‑digit palindromes) */
    }

    /* Extend product cache until we have at least n products */
    while (prod_size <= (size_t)n) {
        uint64_t cand = make_palindrome_odd(next_prefix);
        ++next_prefix;                  /* advance for the next candidate */

        if (is_prime_uint64(cand)) {
            /* Grow the array if necessary */
            if (prod_size == prod_cap) {
                size_t new_cap = prod_cap * 2 + 16;
                seq_t *tmp = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                prod = tmp;
                prod_cap = new_cap;
            }
            seq_t new_prod = prod[prod_size - 1] * (seq_t)cand;
            prod[prod_size] = new_prod;
            ++prod_size;
        }
    }

    /* Return product of first n palindromic primes plus one */
    return prod[(size_t)n] + (seq_t)1;
}
