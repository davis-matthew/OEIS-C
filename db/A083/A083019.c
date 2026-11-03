/*
 * A083019  Number of common divisors of n and F(n) where F(n) denotes the
 *          n-th Fibonacci number.
 * https://oeis.org/A083019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = num_divisors( gcd(n, F(n)) ), where F(n) is the n‑th
 *            Fibonacci number. We compute gcd(n,F(n)) efficiently as gcd(n,
 *            F(n) mod n) and then count its divisors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 8542 (2026-11-01)
 * Helpers:   mulmod_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* Auxiliary types ------------------------------------------------ */
#include <stdint.h>  /* for uint64_t, but we rely on idx_t and seq_t defined by the build */
#include "../utils/mulmod_idx.h"

/* Fast doubling Fibonacci modulo a given modulus.
   Returns F(n) modulo mod. */
static idx_t fib_mod_idx(idx_t n, idx_t mod)
{
    /* Iterative version to avoid recursion depth issues */
    idx_t a = 0;          /* F(0) */
    idx_t b = 1 % mod;    /* F(1) */

    /* Find highest bit of n */
    int shift = 63;
    while (shift >= 0 && ((n >> shift) & 1ULL) == 0)
        shift--;

    for (; shift >= 0; --shift)
    {
        /* Doubling step */
        idx_t t1 = a;
        idx_t t2 = b;
        idx_t m2 = (idx_t)((2ULL * t2) % mod);
        idx_t c = mulmod_idx(t1, (m2 + mod - t1) % mod, mod);       /* c = F(2k)   */
        idx_t d = (mulmod_idx(t1, t1, mod) + mulmod_idx(t2, t2, mod)) % mod; /* d = F(2k+1) */

        if ((n >> shift) & 1ULL)      /* If bit is 1, use F(2k+1) and F(2k+2) */
        {
            a = d;
            b = (c + d) % mod;
        }
        else                          /* If bit is 0, use F(2k) and F(2k+1) */
        {
            a = c;
            b = d;
        }
    }
    return a % mod;   /* F(n) mod mod */
}

/* ------------------------------------------------------------ */

/* Euclidean algorithm for 64‑bit unsigned integers */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count positive divisors of x (x <= 2^64). */
static uint64_t divisor_count(uint64_t x)
{
    if (x <= 1) return 1;     /* divisor count of 0 is not defined, but we never get x==0 */
    uint64_t res = 1;
    uint64_t cnt;

    /* factor out 2 */
    cnt = 0;
    while ((x & 1ULL) == 0)
    {
        x >>= 1;
        ++cnt;
    }
    if (cnt) res *= (cnt + 1);

    /* odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2)
    {
        cnt = 0;
        while (x % p == 0)
        {
            x /= p;
            ++cnt;
        }
        if (cnt) res *= (cnt + 1);
    }
    if (x > 1) res *= 2;   /* remaining prime factor */

    return res;
}

/* ------------------------------------------------------------ */
/* Main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* inactive for indices < 1 */

    /* Compute F(n) modulo n */
    idx_t fmod = fib_mod_idx(n, n);

    /* Compute gcd(n, F(n))  (suffices to use fmod) */
    uint64_t g = gcd64((uint64_t)n, (uint64_t)fmod);

    /* Count divisors of the gcd */
    uint64_t d = divisor_count(g);

    return (seq_t)d;
}
