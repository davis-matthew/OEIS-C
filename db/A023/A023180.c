/*
 * A023180  Numbers k such that Fibonacci(k) == 34 (mod k).
 * https://oeis.org/A023180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that Fibonacci(k) ≡ 34 (mod k)
 *            (offset 1)
 * Range:     u8 n <= 7, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* multiply modulo, using 128‑bit arithmetic */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* fast doubling Fibonacci modulo mod */
static uint64_t fib_mod_uint64(idx_t n, idx_t mod)
{
    if (mod == 1) return 0;
    uint64_t a = 0, b = 1;                 /* F(0)=0, F(1)=1 */
    int l = 63 - __builtin_clzll((unsigned long long)n);
    for (int i = l; i >= 0; --i)
    {
        uint64_t a2 = mul_mod_uint64(a, a, mod);          /* a^2  */
        uint64_t b2 = mul_mod_uint64(b, b, mod);          /* b^2  */
        uint64_t two_b = mul_mod_uint64(b, 2, mod);
        uint64_t two_b_minus_a = two_b >= a ? two_b - a : two_b + mod - a;
        uint64_t c = mul_mod_uint64(a, two_b_minus_a, mod); /* F(2k) */
        uint64_t d = ((unsigned __int128)a2 + b2) % mod;     /* F(2k+1) */
        if ((n >> i) & 1)
        {
            a = d;
            uint64_t t = ((unsigned __int128)c + d) % mod;
            b = t;
        }
        else
        {
            a = c;
            b = d;
        }
    }
    return a;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t len = 0, cap = 0;
    static idx_t checked = 0;

    if (vals == NULL)
    {
        cap = 16;
        vals = (seq_t *)realloc(NULL, sizeof(seq_t) * cap);
    }

    while (len < n)
    {
        ++checked;
        uint64_t k = checked;
        uint64_t required = 34 % k;
        if (fib_mod_uint64(k, k) == required)
        {
            if (len == cap)
            {
                cap *= 2;
                vals = (seq_t *)realloc(vals, sizeof(seq_t) * cap);
            }
            vals[len++] = (seq_t)k;
        }
    }
    return vals[n - 1];
}
