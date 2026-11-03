/*
 * A236563  a(p^n)=p^(n+1)(p-1) if p is prime and a(nm)=lcm(a(n),a(m)) if
 *          gcd(n,m)=1.
 * https://oeis.org/A236563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{p^e‖n} (p^(e+1)*(p-1)) with a(1)=1 (p^e‖n means p^e
 *            is a maximal prime power divisor of n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 986 (2026-11-03)
 * Helpers:   gcd_u64, ipow_idx, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Update lcm factor list: keep the maximum exponent for each prime. */
static void add_or_update(uint64_t prime, uint64_t exp,
                          uint64_t *primes, uint64_t *expns, size_t *cnt)
{
    for (size_t i = 0; i < *cnt; ++i)
    {
        if (primes[i] == prime)
        {
            if (exp > expns[i])
                expns[i] = exp;
            return;
        }
    }
    /* new prime */
    primes[*cnt]   = prime;
    expns[*cnt]    = exp;
    ++(*cnt);
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;

    /* arrays for lcm prime factors; maximum size needed is small (≤128). */
    uint64_t lpr[128];
    uint64_t lex[128];
    size_t lcnt = 0;

    /* factor n and accumulate contributions */
    while (temp > 1)
    {
        uint64_t p = smallest_prime_factor(temp);
        uint64_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        /* contribution of p^e : p^(e+1) */
        add_or_update(p, e + 1, lpr, lex, &lcnt);

        /* contribution of (p-1) */
        uint64_t m = p - 1;
        while (m > 1)
        {
            uint64_t q = smallest_prime_factor(m);
            uint64_t f = 0;
            while (m % q == 0)
            {
                m /= q;
                ++f;
            }
            add_or_update(q, f, lpr, lex, &lcnt);
        }
    }

    /* compute the product of prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < lcnt; ++i)
    {
        seq_t mult = ipow_idx((seq_t)lpr[i], (idx_t)lex[i]);
        result = seq_mul(result, mult);
    }

    return result;
}
