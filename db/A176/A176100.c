/*
 * A176100  Even numbers that are not semiprimes, or, twice the nonprimes.
 * https://oeis.org/A176100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (the (n-1)-th nonprime number), with a(1) = 0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Even numbers that are not semiprimes. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul.h"

/* modular multiplication using 128‑bit arithmetic to avoid overflow */

/* modular exponentiation */
static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (d != 0)
    {
        if (d & 1ULL)
            result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool isPrimeU64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small primes for quick elimination */
    static const uint64_t smallPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,
                                           13ULL,17ULL,19ULL,23ULL,
                                           29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(smallPrimes) / sizeof(smallPrimes[0]); ++i)
    {
        uint64_t p = smallPrimes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool comp = true;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                comp = false;
                break;
            }
        }
        if (comp)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset handling; not used by OEIS */

    idx_t count = 0;
    uint64_t x     = 0;      /* current integer being tested */

    while (true)
    {
        bool nonprime = (x <= 1) || !isPrimeU64(x);
        if (nonprime)
        {
            ++count;
            if (count == n)
                return (seq_t)2 * (seq_t)x;   /* 2×the nonprime */
        }
        ++x;
    }
}
