/*
 * A198045  Numbers n for which bigomega(n) divides Fibonacci(n).
 * https://oeis.org/A198045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number such that bigomega(n) | fib(n) (offset 1)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198045: Numbers n for which bigomega(n) divides Fibonacci(n). */

#include <stdlib.h>
#include <stdio.h>
#include "../utils/bigomega.h"   /* bigomega(idx_t) → seq_t */

/* Compute Fibonacci(n) modulo mod using fast doubling. */
static seq_t fib_mod_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0 % mod;
        *fn1 = 1 % mod;
    } else {
        seq_t a, b;
        fib_mod_pair(n >> 1, mod, &a, &b);
        seq_t t = (b << 1);
        if (t >= mod) t %= mod;
        seq_t c = (a * (t + mod - a)) % mod;    /* F(2k) */
        seq_t d = (a * a + b * b) % mod;        /* F(2k+1) */
        if (n & 1) {
            *fn  = d % mod;
            *fn1 = (c + d) % mod;
        } else {
            *fn  = c % mod;
            *fn1 = d % mod;
        }
    }
    return *fn;
}

static seq_t fib_mod(idx_t n, seq_t mod)
{
    if (mod == 0) return 0;            /* divisor 0 is impossible here */
    seq_t fn, fn1;
    fib_mod_pair(n, mod, &fn, &fn1);
    return fn;
}

seq_t a(idx_t n)
{
    /* Input validation: the sequence starts at n = 1. */
    if (n == 0) return 0;

    /* Static cache: cache[i] = (i+1)-th term. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 1;

    /* Grow cache if necessary. */
    while (cache_len < n) {
        /* Ensure storage space. */
        if (cache_len + 16 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            if (new_cap < cache_len + 16) new_cap = cache_len + 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* realloc failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find next integer satisfying the condition. */
        while (1) {
            ++next_candidate;

            /* bigomega(1) == 0 – skip it. */
            seq_t omega = bigomega(next_candidate);
            if (omega == 0) continue;

            /* Check divisibility. */
            if (fib_mod(next_candidate, omega) == 0) {
                cache[cache_len++] = next_candidate;
                break;
            }
        }
    }

    /* Return the required term. */
    return cache[n - 1];
}
