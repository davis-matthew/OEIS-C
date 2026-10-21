/*
 * A093394  a(n) is the GCD of n and the product of the anti-divisors of n.
 * https://oeis.org/A093394
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, Product of anti‑divisors of n) Computed via a
 *            closed form in terms of the prime factorisation of n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                       /* below offset */

    /* factorise n */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* exponent of 2 in n */
    idx_t e2 = 0;
    size_t odd_start = 0;
    if (cnt > 0 && primes[0] == 2) {
        e2      = exps[0];
        odd_start = 1;
    }

    size_t odd_cnt = cnt - odd_start;          /* number of odd prime factors */

    /* ----- odd n ---------------------------------------------------- */
    if (e2 == 0) {
        if (odd_cnt == 1) {                    /* single odd prime factor */
            idx_t e = exps[odd_start];
            if (e == 1)            /* n is prime */
                return (seq_t)1;
            if ((e & 1) == 0)      /* even exponent */
                return (seq_t)primes[odd_start];
            /* odd exponent ≥3 */
            return (seq_t)n;
        }
        /* more than one odd prime factor (or square‑free) */
        return (seq_t)n;
    }

    /* ----- even n --------------------------------------------------- */
    if (odd_cnt == 0)                          /* power of two */
        return (seq_t)1;

    if (odd_cnt == 1) {                        /* one odd prime factor */
        idx_t p   = primes[odd_start];
        idx_t e   = exps[odd_start];

        /* compute 2^e2 by repeated multiplication (safe for all seq_t) */
        seq_t pow2 = 1;
        for (idx_t i = 0; i < e2; ++i) pow2 *= 2;

        if (e == 1)         /* odd part is a prime */
            return pow2;

        /* odd part is a prime power */
        if (e == 2) {        /* exponent 2 → use only one copy of p */
            return pow2 * (seq_t)p;
        } else {             /* exponent ≥3 → use full power of p */
            seq_t powp = 1;
            for (idx_t i = 0; i < e; ++i) powp *= (seq_t)p;
            return pow2 * powp;
        }
    }

    /* two or more distinct odd prime factors -> a(n) = n */
    return (seq_t)n;
}
