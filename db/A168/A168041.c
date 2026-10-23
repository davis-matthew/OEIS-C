/*
 * A168041  Fibonacci numbers that are divisible by their index.
 * https://oeis.org/A168041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{k_n} where k_n is the n-th index k such that * F_k ≡ 0
 *            (mod k). Off‑set is 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib.h"

/* ----------  helper: fast doubling for Fibonacci modulo a uint64_t modulus  ---------- */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);          /* (F_k , F_{k+1}) */
    /* compute F_{2k} and F_{2k+1} */
    uint64_t t;
    /* t = 2*b mod mod */
    t = b;
    if (t >= mod) t -= mod;       /* t + mod to be safe */
    t = t + t;                    /* 2*b */
    if (t >= mod) t -= mod;       /* mod reduction */
    /* c = a * (t - a) mod mod */
    uint64_t diff;
    if (t >= a)
        diff = t - a;
    else
        diff = t + mod - a;
    uint64_t c = (uint64_t)((__uint128_t)a * diff % mod);
    /* d = a*a + b*b mod mod */
    uint64_t d = (uint64_t)(((__uint128_t)a * a + (__uint128_t)b * b) % mod);
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* ----------  cache for qualifying indices  ---------- */
static idx_t *idx_cache = NULL;  /* array of indices k */
static size_t idx_cache_len = 0;
static size_t idx_cache_cap = 0;
static idx_t checked_up_to = 0;   /* smallest k that has NOT yet been tested */

/* ensure capacity of idx_cache */
static void grow_cache(size_t need)
{
    if (need <= idx_cache_cap)
        return;
    size_t new_cap = idx_cache_cap ? idx_cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    idx_cache = (idx_t *)realloc(idx_cache, new_cap * sizeof(idx_t));
    idx_cache_cap = new_cap;
}

/* extend the cache until it contains at least need indices */
static void fill_cache(idx_t need)
{
    if (idx_cache_len >= (size_t)need)
        return;
    /* start searching from k = 1 if we haven't tested anything yet */
    if (checked_up_to == 0)
        checked_up_to = 1;
    else
        checked_up_to = idx_cache[idx_cache_len - 1] + 1;

    while (idx_cache_len < (size_t)need) {
        uint64_t k = checked_up_to;
        uint64_t rem, rem_next;
        fib_mod_pair(k, k, &rem, &rem_next);  /* compute F_k mod k */
        if (rem == 0) {
            grow_cache(idx_cache_len + 1);
            idx_cache[idx_cache_len++] = k;
        }
        ++checked_up_to;
    }
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* value undefined for n < 1, return 0 */

    fill_cache(n);
    return fib(idx_cache[n - 1]);   /* fib() from utils returns seq_t */
}
