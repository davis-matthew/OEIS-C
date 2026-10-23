/*
 * A023173  Numbers k such that Fibonacci(k) == 1 (mod k).
 * https://oeis.org/A023173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 5030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A023173: Numbers k such that Fibonacci(k) ≡ 1 (mod k).
 *
 * The implementation uses a fast doubling algorithm to compute
 * Fibonacci(n) modulo a given modulus, and a simple incremental
 * search for the next qualifying k.  Results are cached so that
 * calls with arbitrary n are answered in O(1) amortised time.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/mod_mul.h"   /* mod_mul(a,b,mod)  */

static uint64_t fib_mod_u64(idx_t n, uint64_t mod)
{
    uint64_t f = 0;          /* F(0) */
    uint64_t g = 1;          /* F(1) */

    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i) {
        uint64_t f2 = f;
        uint64_t g2 = g;

        /* F(2k) = F(k) * (2*F(k+1) – F(k)) */
        uint64_t two_g   = mod_mul(g2, 2u, mod);
        uint64_t diff    = (two_g + mod - f2) % mod;
        uint64_t c       = mod_mul(f2, diff, mod);

        /* F(2k+1) = F(k)^2 + F(k+1)^2 */
        uint64_t d       = (mod_mul(f2, f2, mod) + mod_mul(g2, g2, mod)) % mod;

        if ((n >> i) & 1) {
            f = d;
            g = (c + d) % mod;
        } else {
            f = c;
            g = d;
        }
    }
    return f;                /* F(n) mod mod */
}

/* Cache of computed terms */
static seq_t *s_cache      = NULL;
static size_t s_cache_cap  = 0;
static idx_t  s_cache_len  = 0;
static idx_t  s_next_k     = 2;   /* next candidate k to test */

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for offset 1; return 0 as fallback */
    }

    if (s_cache == NULL) {
        s_cache_cap = 64;
        s_cache = (seq_t *)malloc(sizeof(seq_t) * s_cache_cap);
        s_cache[0] = (seq_t)1;   /* first term is always 1 */
        s_cache_len = 1;
    }

    while (s_cache_len < n) {
        while (true) {
            uint64_t modv = (uint64_t)s_next_k;
            if (fib_mod_u64(s_next_k, modv) == 1) {
                if (s_cache_len >= s_cache_cap) {
                    s_cache_cap *= 2;
                    s_cache = (seq_t *)realloc(s_cache, sizeof(seq_t) * s_cache_cap);
                }
                s_cache[s_cache_len++] = (seq_t)s_next_k;
                s_next_k++;
                break;
            }
            s_next_k++;
        }
    }

    return s_cache[n - 1];
}
