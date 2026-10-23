/*
 * A017687  Numerator of sum of -12th powers of divisors of n.
 * https://oeis.org/A017687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n} 1/d^12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1625,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Computes p^exp, exp <= 255, using seq_t multiplication.
   On built-in integer seq_t this may overflow; such
   values never produce a valid sequence term for 64‑bit builds. */
static inline seq_t pow_uint(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/* Return the nth term of sequence A017687. */
seq_t a(idx_t n)
{
    /* Handle n==1 separately (no prime factors). */
    if (n == 1) return 1;

    /* Factor n using simple trial division. */
    uint64_t m     = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   pc    = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]  = e;
            ++pc;
        }
    }
    if (m > 1) {
        primes[pc] = m;
        exps[pc]  = 1;
        ++pc;
    }

    const seq_t K   = 12;          /* exponent of the divisor sum */
    seq_t       sigma = 1;         /* sigma_12(n) */

    for (size_t i = 0; i < pc; ++i) {
        seq_t base = (seq_t)primes[i];
        seq_t e    = (seq_t)exps[i];

        /* Compute p^12 once. */
        seq_t pk = pow_uint(base, (unsigned)K);

        /* Compute sigma of p^e: 1 + p^12 + p^(2*12) + ... + p^(e*12). */
        seq_t term  = 1;
        seq_t cur   = 1;
        for (seq_t j = 0; j < e; ++j) {
            cur   = cur * pk;
            term  = term + cur;
        }

        sigma = sigma * term;
    }

    /* Denominator n^12 */
    seq_t denom_n = pow_uint((seq_t)n, (unsigned)K);

    /* Reduce fraction sigma/denom_n. */
    seq_t g = gcd(sigma, denom_n);
    return sigma / g;
}
