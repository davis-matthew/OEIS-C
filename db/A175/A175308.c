/*
 * A175308  Numbers k such that (Sum_{i=0..k} F(i)^2) / (k+1) is an integer,
 *          where F(i) is the i-th Fibonacci number.
 * https://oeis.org/A175308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   k such that Σ_{i=0}^{k} F(i)^2 is divisible by (k+1) where F(i)
 *            is the i‑th Fibonacci number. equivalently: F(k)*F(k+1) mod
 *            (k+1) == 0 a(1) = 0, a(2) = 4, a(3) = 9, …
 * Range:     u8 n <= 47, u16 n <= 3972, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fast doubling Fibonacci modulo mod.
   Computes F(n) and F(n+1) % mod into *fn and *fn1. */
static void fib_mod_uint64(idx_t n, idx_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (mod == 1) {
        *fn  = 0;
        *fn1 = 0;
        return;
    }
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b, c, d;
    fib_mod_uint64(n >> 1, mod, &a, &b);   /* a = F(k), b = F(k+1) */
    /* F(2k) = a * (2*b – a) */
    uint64_t two_b = (uint64_t)(((__int128)2 * b) % mod);
    uint64_t t1   = (two_b + mod - a) % mod;            /* (2*b – a) mod mod */
    c = (uint64_t)((__int128)a * t1 % mod);
    /* F(2k+1) = b*b + a*a */
    d = (uint64_t)(((__int128)b * b + (__int128)a * a) % mod);

    if (n & 1) {
        *fn  = d % mod;                    /* F(2k+1) */
        *fn1 = (c + d) % mod;              /* F(2k+2) */
    } else {
        *fn  = c % mod;                    /* F(2k)   */
        *fn1 = d % mod;                    /* F(2k+1) */
    }
}

/* Returns the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static size_t len = 0, cap = 0;
    static idx_t test_k = 0;                 /* next k to test */

    if (n <= 0)
        return 0;                            /* undefined, just fall back */

    while (len < (size_t)n) {
        idx_t mod = test_k + 1;              /* k+1 */
        uint64_t fk, fk1;

        fib_mod_uint64(test_k, mod, &fk, &fk1);

        if (((unsigned __int128)fk * fk1) % mod == 0) {
            /* Append test_k to list */
            if (len == cap) {
                size_t newcap = (cap == 0 ? 256 : cap << 1);
                seq_t *tmp = (seq_t *)realloc(list, newcap * sizeof(seq_t));
                if (!tmp) {                 /* allocation failure */
                    free(list);
                    list = NULL;
                    cap = len = 0;
                    return 0;
                }
                list = tmp;
                cap = newcap;
            }
            list[len++] = (seq_t)test_k;
        }
        ++test_k;
    }
    return list[n - 1];
}
