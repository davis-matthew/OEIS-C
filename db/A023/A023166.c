/*
 * A023166  Numbers k such that Fibonacci(k) == -8 (mod k).
 * https://oeis.org/A023166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that Fibonacci(k) ≡ -8
 *            (mod k)
 * Range:     u8 n <= 6, u16 n <= 369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the terms sequentially, using fast‑doubling Fibonacci
   modulo k.  It memoises results so that subsequent calls are fast. */
#include <stdlib.h>
#include <stdint.h>

/* fast‑doubling: compute Fibonacci pair (F(n), F(n+1)) modulo 'mod' */
static void fib_pair_u64(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;          /* 1 mod 1 == 0 when mod == 1 */
        return;
    }
    uint64_t a, b;
    fib_pair_u64(n >> 1, mod, &a, &b);

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k))   */
    uint64_t two_b = (unsigned __int128)b * 2 % mod;
    uint64_t two_b_minus_a = (two_b + mod - a) % mod;
    uint64_t c = (unsigned __int128)a * two_b_minus_a % mod;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2  */
    uint64_t d = ((unsigned __int128)a * a + (unsigned __int128)b * b) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        uint64_t sum = (unsigned __int128)c + d % mod;
        /* compute (c + d) modulo mod safely */
        uint64_t sum_mod = (unsigned __int128)(c) + d;
        *fn1 = (uint64_t)(sum_mod % mod);
    }
}

/* compute F(n) mod mod */
static uint64_t fib_mod_u64(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_u64(n, mod, &fn, &fn1);
    return fn;
}

/* The entry function required by the build system */
seq_t a(idx_t n)
{
    static seq_t *terms     = NULL;
    static size_t allocated = 0;
    static size_t len       = 0;
    static idx_t last_k     = 1;   /* next candidate to test */

    if (n == 0) return (seq_t)0;   /* out of range, but never called */

    while (len < (size_t)n) {
        /* grow the terms array if needed */
        if (len == allocated) {
            size_t new_cap = allocated ? allocated * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            terms = tmp;
            allocated = new_cap;
        }

        /* test candidate k = last_k */
        uint64_t k_mod = (uint64_t)last_k;
        uint64_t fmod = fib_mod_u64(k_mod, k_mod);

        if ((fmod + 8U) % k_mod == 0U) {
            terms[len++] = (seq_t)last_k;
        }
        last_k++;
    }
    return terms[n - 1];
}
