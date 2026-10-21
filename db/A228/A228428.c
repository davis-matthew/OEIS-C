/*
 * A228428  Number of ways to write n = x + y (x, y > 0) with p(99, x) +
 *          p(100, y) prime, where p(m, k) denotes the m-gonal number
 *          (m-2)*k*(k-1)/2 + k.
 * https://oeis.org/A228428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 7169, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   polygonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for all decompositions n = x + y (x,y>0) such that
   p(99,x) + p(100,y) is prime, where p(m,k) = (m-2)k(k-1)/2 + k. */

/* the build supplies the types seq_t and idx_t (unsigned 64‑bit) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/polygonal.h"

/* fast 64‑bit modular multiplication (uses the optional 128‑bit type */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* fast 64‑bit modular exponentiation */
static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e)
    {
        if (e & 1UL) res = mod_mul64(res, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit integers, correct for all n < 2^64 */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    const idx_t sp_cnt =
        sizeof(small_primes) / sizeof(small_primes[0]);

    for (idx_t i = 0; i < sp_cnt; ++i)
    {
        uint64_t sp = small_primes[i];
        if (n == sp) return true;
        if (n % sp == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for all 64‑bit integers */
    const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    const idx_t b_cnt =
        sizeof(bases) / sizeof(bases[0]);

    for (idx_t i = 0; i < b_cnt; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* public sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x)
    {
        idx_t y = n - x;
        seq_t s = polygonal(99, x) + polygonal(100, y);
        /* s fits in 64‑bit, so cast for the primality test */
        if (is_prime_uint64((uint64_t)s))
            ++count;
    }
    return count;
}
