/*
 * A067695  Smallest prime factor with minimum exponent in canonical prime
 *          factorization of n, a(1)=1.
 * https://oeis.org/A067695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor having the minimum exponent in the
 *            canonical prime factorisation of n. a(1)=1
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* a(1) is defined as 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factor n into prime list (primes[i]) and exponent list (exps[i]).
       n ≤ 2^64‑1, so an array of 64 entries is sufficient. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor((uint64_t)n, primes, exps, &cnt);

    /* Find the minimum exponent among the primes. */
    uint64_t min_exp = UINT64_MAX;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] < min_exp) {
            min_exp = exps[i];
        }
    }

    /* Among all primes that have the minimum exponent, choose the smallest. */
    uint64_t min_prime = 0;  /* 0 will mean “not yet set”. */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == min_exp) {
            if (min_prime == 0 || primes[i] < min_prime) {
                min_prime = primes[i];
            }
        }
    }

    return (seq_t)min_prime;
}
