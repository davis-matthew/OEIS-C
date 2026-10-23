/*
 * A105190  Number of distinct prime divisors of n100000000001.
 * https://oeis.org/A105190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct prime divisors of n*10^12 +
 *            100000000001
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   gcd_u64, insertion_sort_u64, mul_mod_u64, pollard_rho_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/insertion_sort_u64.h"
#include "../utils/mul_mod_u64.h"
#include "../utils/pollard_rho_u64.h"

/* ------------------------------------------------------------------ */
/*  Modular arithmetic for 64‑bit numbers                               */
/* ------------------------------------------------------------------ */

static uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Greatest common divisor                                            */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Deterministic Miller–Rabin for 64‑bit                                */
/* ------------------------------------------------------------------ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;

    static const uint64_t small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i)
    {
        uint64_t a = small_primes[i];
        if (a >= n)
            continue;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1)
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

/* ------------------------------------------------------------------ */
/*  Pollard–Rho factorisation                                          */

/* ------------------------------------------------------------------ */
/*  Recursive factorisation into an array                              */
/* ------------------------------------------------------------------ */
static void factor_u64(uint64_t n, uint64_t *factors, size_t *cnt)
{
    uint64_t stack[64];
    size_t top = 0;
    stack[top++] = n;
    *cnt = 0;

    while (top)
    {
        uint64_t m = stack[--top];
        if (m == 1)
            continue;
        if (is_prime_u64(m))
        {
            if (*cnt < 64)
                factors[(*cnt)++] = m;
            continue;
        }
        uint64_t d = pollard_rho_u64(m);
        stack[top++] = d;
        stack[top++] = m / d;
    }
}

/* ------------------------------------------------------------------ */
/*  Simple insertion sort for small arrays                             */

/* ------------------------------------------------------------------ */
/*  Sequence function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Compute M = n*10^12 + 100000000001.  For n <= 10000 this fits in 64 bits. */
    const uint64_t TEN12 = 1000000000000ULL;
    const uint64_t OFF   = 100000000001ULL;
    uint64_t M = (uint64_t)(n * TEN12 + OFF);

    /* Factor M */
    uint64_t factors[64];
    size_t fac_cnt;
    factor_u64(M, factors, &fac_cnt);

    if (fac_cnt == 0)          /* M == 1, impossible here, but keep code safe */
        return (seq_t)0;

    insertion_sort_u64(factors, fac_cnt);

    /* Count distinct prime factors */
    size_t distinct = 1;
    for (size_t i = 1; i < fac_cnt; ++i)
        if (factors[i] != factors[i - 1])
            ++distinct;

    return (seq_t)distinct;
}
