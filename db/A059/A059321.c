/*
 * A059321  Smallest number m such that m^2+1 is divisible by A002144(n)^2 (=
 *          squares of primes congruent to 1 mod 4).
 * https://oeis.org/A059321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 4539, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the least positive integer m such that
   m^2 + 1 is divisible by the square of the n‑th prime congruent
   to 1 mod 4. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Modular exponentiation for 64‑bit integers */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Return a square root of −1 modulo the prime p (p ≡ 1 mod 4) using
   the Tonelli–Shanks algorithm.  The result lies in 0 < x < p. */
static uint32_t sqrt_minus_one_prime(uint32_t p)
{
    uint64_t q = p - 1;
    uint32_t s = 0;
    while ((q & 1ULL) == 0)
    {
        q >>= 1ULL;
        ++s;
    }

    /* Find a quadratic non‑residue z */
    uint32_t z = 2;
    while (powmod_uint64(z, (p - 1) / 2, p) != p - 1)
        ++z;

    uint64_t c = powmod_uint64(z, q, p);
    uint64_t r = powmod_uint64(p - 1, (q + 1) / 2, p);
    uint64_t t = powmod_uint64(p - 1, q, p);
    uint32_t m = s;

    while (t != 1)
    {
        uint64_t tt = t;
        uint32_t i;
        for (i = 1; i < m; ++i)
        {
            tt = (tt * tt) % p;
            if (tt == 1)
                break;
        }
        uint64_t b = powmod_uint64(c, 1ULL << (m - i - 1), p);
        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return (uint32_t)r;
}

/* Cache of primes ≡ 1 (mod 4). */
static uint32_t *primes_C = NULL;
static size_t primes_cap_C = 0;
static size_t primes_cnt_C = 0;

/* Generate enough primes ≡ 1 (mod 4) to reach index n. */
static void ensure_primes_C(size_t n)
{
    while (primes_cnt_C < n)
    {
        uint64_t start = (primes_cnt_C == 0) ? 3 : (uint64_t)primes_C[primes_cnt_C - 1];
        uint64_t p = next_prime(start);
        while (p % 4ULL != 1ULL)
            p = next_prime(p);

        if (primes_cnt_C == primes_cap_C)
        {
            size_t new_cap = (primes_cap_C == 0) ? 64 : primes_cap_C * 2;
            uint32_t *tmp = (uint32_t *)realloc(primes_C, new_cap * sizeof(uint32_t));
            primes_C = tmp;
            primes_cap_C = new_cap;
        }
        primes_C[primes_cnt_C++] = (uint32_t)p;
    }
}

/* Sequence function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* outside the defined offset */

    /* Obtain the n‑th prime ≡ 1 (mod 4) */
    ensure_primes_C((size_t)n);
    uint32_t p_uint = primes_C[n - 1];
    seq_t p = (seq_t)p_uint;

    /* Find a square root of –1 modulo p */
    uint32_t k = sqrt_minus_one_prime(p_uint);

    /* Hensel lift to modulo p² */
    uint64_t prod = (uint64_t)k * k;          /* k²  */
    uint64_t c = (prod + 1ULL) / p_uint;      /* (k²+1)/p  */
    uint64_t two_k_mod = ((uint64_t)2 * k) % p_uint;
    uint64_t inv2k = powmod_uint64(two_k_mod, p_uint - 2ULL, p_uint);
    uint64_t t_val = ((p_uint - (c % p_uint)) % p_uint) * inv2k % p_uint;

    seq_t x = (seq_t)k + (seq_t)t_val * p;    /* solution modulo p² */
    seq_t p2 = p * p;
    seq_t alt = p2 - x;                       /* the other root   */

    return (x <= alt) ? x : alt;
}
