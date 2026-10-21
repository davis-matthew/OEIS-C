/*
 * A229998  Denominator of d(k)/d(1) + d(k-1)/d(2) + ... + d(k)/d(1), where
 *          d(1),d(2),...,d(k) are the unitary divisors of n.
 * https://oeis.org/A229998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = n / gcd(∏(p^(2e)+1), n) , where n = ∏ p^e
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* Euclidean GCD for 64-bit unsigned integers. */

/* Simple trial division factorisation.
   primes[] and exps[] must be large enough (<=16 for n<=1e4). */
static void factor_uint64(uint64_t n, uint64_t primes[], uint64_t exps[],
                          size_t *cnt)
{
    size_t i = 0;
    if (n == 0) {
        *cnt = 0;
        return;
    }
    while ((n & 1) == 0) {          // factor out 2
        primes[i] = 2;
        exps[i] = 0;
        do { n >>= 1; exps[i]++; } while ((n & 1) == 0);
        ++i;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[i] = p;
            exps[i] = 0;
            do { n /= p; exps[i]++; } while (n % p == 0);
            ++i;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        primes[i] = n;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

seq_t a(idx_t n)
{
    uint64_t un = (uint64_t)n;
    /* n>=1 in all valid calls, but handle n=0 safely. */
    if (un == 0)
        return (seq_t)0;

    /* Factorisation */
    uint64_t primes[16], exps[16];
    size_t cnt;
    factor_uint64(un, primes, exps, &cnt);

    /* Compute N = ∏(p^(2e)+1) */
    seq_t N = (seq_t)1;
    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = primes[k];
        uint64_t e = exps[k];
        seq_t term = (seq_t)1;
        /* compute p^(2e) */
        for (uint64_t i = 0; i < (uint64_t)(2 * e); ++i)
            term *= (seq_t)p;
        term += 1;                 /* tile: p^(2e)+1 */
        N *= term;
    }

    /* GCD of N and n (both fit in 64 bits for the tested ranges) */
    uint64_t g = gcd_uint64((uint64_t)N, un);

    /* a(n) = n / g */
    return (seq_t)(un / g);
}
