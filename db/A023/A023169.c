/*
 * A023169  Numbers k such that Fib(k) == -34 (mod k).
 * https://oeis.org/A023169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k with Fib(k) ≡ –34 (mod k)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* modular multiplication using 128‑bit to avoid overflow */
static inline uint64_t mul_mod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((__uint128_t)x * y) % m);
}

/* Fibonacci modulo m using fast doubling (iterative) */
static inline uint64_t fib_mod_uint64(uint64_t n, uint64_t m)
{
    if (m == 1) return 0;          /* all Fibonacci numbers ≡ 0 (mod 1) */
    if (n == 0) return 0;
    uint64_t a = 0;  /* F(0) */
    uint64_t b = 1;  /* F(1) */

    /* process bits of n from most significant to least */
    for (int i = 63 - __builtin_clzll(n); i >= 0; --i)
    {
        /* doubling formulas:
           F(2k)   = F(k) * [2*F(k+1) – F(k)]
           F(2k+1) = F(k)^2 + F(k+1)^2        */
        uint64_t t1 = mul_mod(a, ( ( (b << 1) % m + m - a ) % m ), m); /* F(2k) */
        uint64_t t2 = ( ( mul_mod(a, a, m) + mul_mod(b, b, m) ) % m );   /* F(2k+1) */

        if ((n >> i) & 1U) {                      /* odd */
            a = t2;
            b = (t1 + t2) % m;
        } else {                                  /* even */
            a = t1;
            b = t2;
        }
    }
    return a % m;
}

/* static cache of found k values */
static seq_t *cache   = NULL;      /* array of seq_t */
static size_t cache_len = 0;       /* number of elements stored */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  last_k    = 0;       /* largest k already tested */

/* Function to return the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but harmless */

    /* enlarge cache if necessary */
    while (cache_len < (size_t)n)
    {
        last_k++;
        uint64_t k = (uint64_t)last_k;
        uint64_t fibk = fib_mod_uint64(k, k);       /* Fib(k) mod k */
        if ( (fibk + 34U) % k == 0U )               /* Fib(k) ≡ –34 (mod k) */
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc((void *)cache,
                                                new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
