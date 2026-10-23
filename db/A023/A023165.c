/*
 * A023165  Numbers k such that Fibonacci(k) == -5 (mod k).
 * https://oeis.org/A023165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are all k such that Fibonacci(k) ≡ -5 (mod k). The function
 *            iterates k=1,2,3,… and tests the congruence by computing F(k)
 *            modulo k with a fast‑doubling algorithm. Results are memoised in
 *            a static array so any order of calls is supported
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t fib_mod_uint64(idx_t n, uint64_t mod)
{
    /* Fast doubling to compute F(n) mod mod.  Works for any mod>=1. */
    uint64_t a = 0; /* F(0) */
    uint64_t b = 1; /* F(1) */
    for (int i = 63; i >= 0; --i)
    {
        /* c = F(2k)   = F(k) * (2*F(k+1) – F(k)) */
        uint64_t two_b = (unsigned __int128)2 * b % mod;
        uint64_t diff  = (two_b >= a) ? (two_b - a) : (two_b + mod - a);
        uint64_t c     = mul_mod_uint64(a, diff, mod);

        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
        uint64_t d = (mul_mod_uint64(a, a, mod) + mul_mod_uint64(b, b, mod)) % mod;

        if (((n >> i) & 1) == 0)
        {
            a = c;
            b = d;
        }
        else
        {
            a = d;
            b = (c + d) % mod;
        }
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static cache for previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t tested_up_to = 0;   /* last k tested */

    if (n <= cache_len)
        return cache[n-1];

    /* Extend cache until we have at least n terms. */
    while (cache_len < n)
    {
        ++tested_up_to;
        uint64_t mod = (uint64_t)tested_up_to;
        uint64_t f_mod = fib_mod_uint64(tested_up_to, mod);
        if ((f_mod + 5) % mod == 0)
        {
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (new_arr == NULL)
                    abort();   /* out of memory */
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)tested_up_to;
        }
    }
    return cache[n-1];
}
