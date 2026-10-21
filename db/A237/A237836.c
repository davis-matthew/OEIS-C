/*
 * A237836  Pisano period of n^2.
 * https://oeis.org/A237836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001175(n^2) – Pisano period of n²
 * Range:     u8 n <= 9, u16 n <= 174, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_pair_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------------- */
/*  Utilities and helpers                                                    */
/* -------------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/fib_pair_u64.h"

/* compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* -------------------------------------------------------------------------- */
/*  Fast doubling Fibonacci modulo a 64‑bit modulus                           */

static uint64_t fib_mod_u64(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_u64(n, mod, &fn, &fn1);
    return fn;
}

/* -------------------------------------------------------------------------- */
/*  Pisano period of a prime p (p ≥ 2)                                        */
/* -------------------------------------------------------------------------- */
static seq_t period_prime_u64(uint64_t p)
{
    if (p == 2)  return (seq_t)3;
    if (p == 5)  return (seq_t)20;

    unsigned int mod5 = p % 5;
    uint64_t t0 = (mod5 == 1 || mod5 == 4) ? (p - 1) : (2 * (p + 1));

    /* collect divisors of t0 */
    uint64_t divs[256];
    size_t dc = 0;
    for (uint64_t d = 1; d * d <= t0; ++d) {
        if (t0 % d == 0) {
            divs[dc++] = d;
            if (d != t0 / d)
                divs[dc++] = t0 / d;
        }
    }

    /* sort ascending */
    qsort(divs, dc, sizeof(uint64_t), cmp_uint64);

    for (size_t i = 0; i < dc; ++i) {
        uint64_t d = divs[i];
        if (fib_mod_u64(d, p) == 0 && fib_mod_u64(d + 1, p) == 1)
            return (seq_t)d;
    }
    /* Should never reach here for prime p */
    return (seq_t)0;
}

/* -------------------------------------------------------------------------- */
/*  Pisano period of a prime power p^e, e ≥ 1                                 */
/* -------------------------------------------------------------------------- */
static seq_t period_prime_power(idx_t p, idx_t e)
{
    seq_t period = 0;

    if (p == 2) {                /* period(2^e) = 3 * 2^(e-1) */
        period = (seq_t)3;
        for (idx_t i = 1; i < e; ++i) period *= 2U;
    } else if (p == 5) {         /* period(5^e) = 20 * 5^(e-1) */
        period = (seq_t)20;
        for (idx_t i = 1; i < e; ++i) period *= 5U;
    } else {
        seq_t period_prime = period_prime_u64((uint64_t)p);
        period = period_prime;
        for (idx_t i = 1; i < e; ++i) period *= (seq_t)p;
    }
    return period;
}

/* -------------------------------------------------------------------------- */
/*  Main sequence function                                                   */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    idx_t tmp = n;
    seq_t result = 1;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            idx_t e = 2 * exp;                /* exponent in n² */
            seq_t pwr = period_prime_power(p, e);
            seq_t g = gcd(result, pwr);
            result = (result / g) * pwr;
        }
    }

    if (tmp > 1) {                      /* remaining prime factor */
        idx_t e = 2;                     /* exponent in n² is 2 */
        seq_t pwr = period_prime_power(tmp, e);
        seq_t g = gcd(result, pwr);
        result = (result / g) * pwr;
    }

    return result;
}
