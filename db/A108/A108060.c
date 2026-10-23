/*
 * A108060  Numbers k such that 191*k + 193 is prime.
 * https://oeis.org/A108060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 191*k + 193 is prime (offset 1) */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*   64‑bit modular multiplication using 128‑bit intermediate    */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/*   64‑bit modular exponentiation                               */
static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1ULL;
    }
    return r;
}

/*   Deterministic Miller–Rabin test for 64‑bit integers          */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    size_t i;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n‑1 as d·2ᵖ with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1ULL;
        ++s;
    }

    /* bases that work for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};

    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        uint64_t r;
        for (r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*   a(n) = the nth integer k (offset 1) with 191*k + 193 prime   */
seq_t a(idx_t n)
{
    idx_t   count = 0;          /* how many primes found so far   */
    uint64_t k    = 0;          /* current candidate k           */
    const uint64_t factor = 191ULL;
    const uint64_t offset = 193ULL;

    while (count < n)
    {
        uint64_t p = factor * k + offset;   /* test number   */
        if (isPrime64(p)) ++count;
        if (count == n) return (seq_t)k;    /* found the nth k */
        ++k;
    }
    /* never reached */
    return (seq_t)0;
}
