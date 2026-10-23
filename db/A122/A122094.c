/*
 * A122094  Prime divisors of Mersenne numbers. Primes p such that the
 *          multiplicative order of 2 modulo p is prime.
 * https://oeis.org/A122094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A122094 – primes p such that the multiplicative order of 2
 *            modulo p is prime. ** Elements are generated in increasing
 *            order. The implementation keeps a cache of ** already computed
 *            terms and a list of primes found so far. The cache and prime **
 *            tables are allocated once with a fixed maximum size...
 * Range:     u8 n <= 10, u16 n <= 412, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*  Parameters – enlarge if more terms than the first 10 000 are requested.   */
/* ------------------------------------------------------------------------- */
#define MAX_CACHE   20000     /* must be >= maximum n that may be queried */
#define MAX_PRIMES  300000    /* number of primes to keep – plenty for 10 000 terms */

/* ------------------------------------------------------------------------- */
/*  Sequence data – static so that calls can be made in any order              */
/* ------------------------------------------------------------------------- */
static seq_t  cache[MAX_CACHE];
static size_t cache_len = 0;

static uint64_t primes[MAX_PRIMES];
static size_t   primes_len = 0;
static uint64_t next_candidate = 3;

/* ------------------------------------------------------------------------- */
/*  Tiny helpers for 64‑bit modular arithmetic                                  */
/* ------------------------------------------------------------------------- */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------------- */
/*  Primality test – uses the primes that are already known                    */
/* ------------------------------------------------------------------------- */
static bool is_prime_small(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i)
    {
        uint64_t p = primes[i];
        if (p > n / p)       /* p*p > n  –  no further trial division needed */
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/*  Ensure that we have computed at least n terms of the sequence.            */
/* ------------------------------------------------------------------------- */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* One‑time prime table setup.  The first prime is 2. */
    if (primes_len == 0)
    {
        primes[0] = 2ULL;
        primes_len = 1;
        next_candidate = 3ULL;
    }

    /* Generate primes and test them until we have n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime candidate. */
        uint64_t p = next_candidate;
        while (!is_prime_small(p))
            p += 2ULL;
        next_candidate = p + 2ULL;       /* advance candidate for the next round */

        /* Record the prime for use in later factorizations. */
        if (primes_len < MAX_PRIMES)
            primes[primes_len++] = p;
        else
            return;   /* should never happen for the requested limits */

        /* --- Compute the multiplicative order of 2 modulo p --- */
        uint64_t ord = p - 1;
        uint64_t temp = ord;
        uint64_t factors[64];
        size_t   fcnt = 0;

        /* Factor ord into distinct prime factors. */
        for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= temp; ++i)
        {
            uint64_t q = primes[i];
            if (temp % q == 0)
            {
                factors[fcnt++] = q;
                while (temp % q == 0)
                    temp /= q;
            }
        }
        if (temp > 1)
            factors[fcnt++] = temp;

        /* Reduce ord by removing prime factors where possible. */
        for (size_t i = 0; i < fcnt; ++i)
        {
            uint64_t q = factors[i];
            while (ord % q == 0 && mod_pow_uint64(2ULL, ord / q, p) == 1ULL)
                ord /= q;
        }

        /* If the resulting order is prime, p belongs to the sequence. */
        if (is_prime_small(ord))
            cache[cache_len++] = (seq_t)p;
    }
}

/* ------------------------------------------------------------------------- */
/*  Public function – external linkage required.                            */
/* ------------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)           /* sequence is 1‑based – 0 is a harmless guard */
        return 0;
    ensure_terms(n);      /* compute terms up to n if necessary */
    return cache[n - 1];
}
