/*
 * A108935  Numbers k such that 7*k + 911 is prime.
 * https://oeis.org/A108935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th even integer k such that 7*k + 911 is prime. */
/* Uses a deterministic Miller–Rabin test for 64‑bit integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 64‑bit modular multiplication using 128‑bit intermediate. */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation. */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = modmul64(r, a, mod);
        a = modmul64(a, a, mod);
        e >>= 1U;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;

    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0ULL)
            return false;
    }

    /* Write n-1 = d * 2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1U;
        ++s;
    }

    /* Witness loop. */
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t a = small_primes[i];
        if (a >= n)
            break;

        uint64_t x = modpow64(a, d, n);
        if (x == 1ULL || x == n - 1ULL)
            continue;

        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = modmul64(x, x, n);
            if (x == n - 1ULL)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }

    return true;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not used with offset 1, harmless */

    idx_t seen = 0;                  /* number of terms found so far */
    uint64_t k = 0;                  /* candidate even integer */

    while (true)
    {
        uint64_t test = 7ULL * k + 911ULL;  /* 7*k + 911 */
        if (is_prime_u64(test))
        {
            ++seen;
            if (seen == n)
                return (seq_t)k;
        }
        k += 2;                       /* next even integer */
    }
}
