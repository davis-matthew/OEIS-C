/*
 * A023168  Numbers k such that Fib(k) == -21 (mod k).
 * https://oeis.org/A023168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that Fib(k) == -21 (mod k)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* fast‑doubling Fibonacci modulo */
static void fib_pair_u64(idx_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_u64(n >> 1, mod, &a, &b);          /* a = F_k, b = F_{k+1} */
    uint64_t two_b = (b << 1) % mod;             /* 2*b mod mod */
    uint64_t temp  = (two_b + mod - a % mod) % mod; /* (2*b - a) mod mod */
    uint64_t c = mod_mul(a, temp, mod);              /* F_{2k} */
    uint64_t d = (mod_mul(a, a, mod) + mod_mul(b, b, mod)) % mod; /* F_{2k+1} */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

static uint64_t fib_mod_u64(idx_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_u64(n, mod, &fn, &fn1);
    return fn;
}

/* static storage for found terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0, cache_cap = 0;
static idx_t next_k       = 1;   /* next k to test */

/* helper: ensure capacity of cache */
static void grow_cache(void)
{
    if (cache_cap == cache_size) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* lazily generate terms until we have n terms */
    while (cache_size < n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t fk = fib_mod_u64(k, k);          /* F_k mod k */
        if ((fk + 21) % k == 0) {                 /* condition F_k ≡ -21 (mod k) */
            grow_cache();
            cache[cache_size++] = (seq_t)k;
        }
        next_k++;
    }
    return cache[n - 1];
}
