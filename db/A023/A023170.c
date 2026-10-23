/*
 * A023170  Numbers k such that Fibonacci(k) == -55 (mod k).
 * https://oeis.org/A023170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that Fibonacci(k) ≡ –55
 *            (mod k)
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches for k incrementally, computing Fib(k) mod k by fast doubling. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Multiply two unsigned integers modulo mod using 128‑bit arithmetic to avoid overflow. */
static inline idx_t mul_mod_idx(idx_t a, idx_t b, idx_t mod)
{
    return (idx_t)(((__uint128_t)a * b) % mod);
}

/* Fast doubling: computes (F(n), F(n+1)) modulo mod. */
static void fib_pair(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    idx_t a, b;
    fib_pair(n >> 1, mod, &a, &b);
    idx_t two_b = mul_mod_idx(2, b, mod);
    idx_t two_b_minus_a;
    if (two_b >= a)
        two_b_minus_a = two_b - a;
    else
        two_b_minus_a = mod + two_b - a;
    idx_t c = mul_mod_idx(a, two_b_minus_a, mod);
    idx_t d = (mul_mod_idx(a, a, mod) + mul_mod_idx(b, b, mod)) % mod;
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* Return Fib(n) modulo mod. */
static idx_t fib_mod_idx(idx_t n, idx_t mod)
{
    idx_t fn, fn1;
    fib_pair(n, mod, &fn, &fn1);
    return fn;
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* the OEIS offset is 1, guard anyway */

    static idx_t *terms = NULL;          /* memoised list of k’s */
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t next_k = 1;             /* next k to test */

    while (len < n) {
        idx_t k = next_k;
        /* Fib(k) modulo k */
        idx_t fibk = fib_mod_idx(k, k);

        /* Check if k divides Fib(k)+55 */
        idx_t rem = (idx_t)((( __uint128_t)fibk + 55) % k);
        if (rem == 0) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                if (!tmp) { /* allocation failure, unlikely, return 0 as safe value */
                    free(terms);
                    terms = NULL;
                    len = cap = next_k = 0;
                    return 0;
                }
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = k;
        }
        ++next_k;
    }

    /* Convert index type to seq_t before returning */
    return (seq_t)terms[n - 1];
}
