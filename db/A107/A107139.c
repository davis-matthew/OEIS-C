/*
 * A107139  Primes of the form 2x^2 + 17y^2.
 * https://oeis.org/A107139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 2x^2 + 17y^2
 * Range:     u8 n <= 7, u16 n <= 813, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* modular multiplication (uint64_t) with 128‑bit intermediate */
static uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t res = ( __uint128_t)a * b % mod;
    return (uint64_t)res;
}

/* modular exponentiation (uint64_t) */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1) r = mul64_mod(r, a, mod);
        a = mul64_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r)
        {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* integer square root of uint64_t */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t x = (uint64_t)sqrt((double)n);
    while ((x + 1) * (x + 1) <= n) ++x;
    while (x * x > n) --x;
    return x;
}

/* test whether a prime p can be written as 2x^2 + 17y^2 */
static bool good_form(uint64_t p)
{
    if (p == 2) return true;
    uint64_t maxy = isqrt_uint64(p / 17);
    for (uint64_t y = 1; y <= maxy; ++y)
    {
        uint64_t t = p - 17 * y * y;
        if (t & 1ULL) continue;
        uint64_t x2 = t >> 1;
        uint64_t x = isqrt_uint64(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Sequence function: a(idx_t n)                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t sz = 0, cap = 0;
    static uint64_t cur = 3;           /* next odd number to test */

    if (sz == 0)                     /* first call – initialise */
    {
        cap = 8;
        seq = (seq_t*)calloc(cap, sizeof(seq_t));
        seq[0] = (seq_t)2;          /* a(1) = 2   */
        sz = 1;
    }

    /* generate until we have the n‑th term */
    while (sz < n)
    {
        if (is_prime_uint64(cur) && good_form(cur))
        {
            if (sz == cap)
            {
                cap <<= 1;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
            }
            seq[sz++] = (seq_t)cur;
        }
        cur += 2;   /* only odd candidates needed after 2 */
    }

    return seq[n - 1];
}
