/*
 * A127787  Numbers n such that F(n) divides F(F(n)), where F(n) is a
 *          Fibonacci number.
 * https://oeis.org/A127787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mul_mod_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer n such that Fibonacci(n) divides
   Fibonacci(Fibonacci(n)).  For n>2 this is equivalent to n | F(n). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod_64.h"

/* Multiply two 64‑bit values modulo mod using unsigned __int128
   to avoid overflow.  This works on C++17 and supported C11 compilers. */

/* Compute Fibonacci(n) modulo mod using fast doubling (iterative). */
static uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    if (mod == 1) return 0;            /* everything is 0 mod 1 */
    uint64_t a = 0, b = 1;             /* F(0), F(1) */

    /* Highest set bit index of n (n>0). */
    int bits = 63 - __builtin_clzll(n);
    for (int i = bits; i >= 0; --i)
    {
        /* Basic doubling formulas:
           c = F(2k)   = F(k) * (2*F(k+1) – F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2          */
        uint64_t two_b = b + b;
        if (two_b >= mod) two_b -= mod;

        /* two_b – a modulo mod */
        uint64_t two_b_minus_a;
        if (two_b >= a)
            two_b_minus_a = two_b - a;
        else
            two_b_minus_a = two_b + mod - a;

        uint64_t c = mul_mod_64(a, two_b_minus_a, mod);
        uint64_t d = mul_mod_64(a, a, mod) + mul_mod_64(b, b, mod);
        if (d >= mod) d -= mod;

        if ((n >> i) & 1ULL) {           /* odd exponent bit */
            a = d;
            uint64_t sum = c + d;
            if (sum >= mod) sum -= mod;
            b = sum;
        } else {
            a = c;
            b = d;
        }
    }
    return a % mod;    /* F(n) mod mod */
}

/* ------- Dynamic cache of sequence terms -------- */
static seq_t *cache = NULL;    /* table of found terms */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------- The sequence function -------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* below offset; not used */

    /* Keep track of the largest n we've tested so far. */
    static idx_t tested_upto = 0;

    while (cache_len < (size_t)n)
    {
        ++tested_upto;   /* next candidate integer */

        bool ok;
        if (tested_upto <= 2)
            ok = true;   /* F(1)=F(2)=1 always divides everything */
        else
            ok = (fib_mod_uint64((uint64_t)tested_upto,
                                 (uint64_t)tested_upto) == 0);

        if (ok)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)tested_upto;
        }
    }
    return cache[n - 1];
}
