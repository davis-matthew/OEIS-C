/*
 * A106958  Primes of the form 4x^2-xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 4x² – x·y + 5y²  (x,y ≥ 0) */
/* Computation: a(n) is found by searching successive primes and testing
   representability; the function works for any seq_t and idx_t in the
   provided build. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mulmod64.h"

/* multiply modulo 64‑bit integer without overflow – used only in Miller–Rabin */

/* modular exponentiation (a^e mod mod) */
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin test for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (unsigned i = 0; i < sizeof small / sizeof small[0]; ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
    { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };
    for (unsigned i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* integer square root of a 128‑bit value – result fits in 64 bits */
static unsigned long long isqrt128(unsigned __int128 x)
{
    /* use long double approx then correct */
    unsigned long long r = (unsigned long long)sqrtl((long double)x);
    unsigned __int128 sq = (unsigned __int128)r * r;
    while (sq < x) { ++r; sq = (unsigned __int128)r * r; }
    while (sq > x) { --r; sq = (unsigned __int128)r * r; }
    return r;
}

/* test whether a prime p can be represented as 4x² – x·y + 5y² with x,y ≥ 0 */
static bool representable(uint64_t p)
{
    if (p < 5) return false; /* 5 is the smallest prime of this form */
    unsigned __int128 lim = (unsigned __int128)16 * p / 79;
    unsigned long long y_max = isqrt128(lim);

    for (unsigned long long y = 0; y <= y_max; ++y)
    {
        unsigned __int128 yy = (unsigned __int128)y * y;
        unsigned __int128 D  = (unsigned __int128)16 * p - (unsigned __int128)79 * yy;
        unsigned long long sqrtD = isqrt128(D);
        if ((unsigned __int128)sqrtD * sqrtD != D) continue;
        if ((y + sqrtD) % 8 == 0) return true;
        if (y >= sqrtD && (y - sqrtD) % 8 == 0) return true;
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t count = 0;
    for (uint64_t candidate = 5; ; candidate += 2)   /* 5 is the first candidate prime */
    {
        if (!isPrime64(candidate)) continue;
        if (representable(candidate))
        {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
    }
}
