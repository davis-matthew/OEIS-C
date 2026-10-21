/*
 * A060567  Number of binomial coefficients C(n,j) with j=0..n that are
 *          divisible by C(n,2).
 * https://oeis.org/A060567
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   add_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = # { j | 0 ≤ j ≤ n, C(n,2) divides C(n,j) }   */

/* The implementation factors C(n,2) as the product of n and (n‑1),
   then checks the p‑adic valuation of each binomial coefficient
   C(n,j) against the required exponents.  The algorithm runs in
   O(n · d) time for a single n, where d is the number of distinct
   prime factors of C(n,2).  This easily meets the time limits for
   n ≤ 10000. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* below the OEIS offset */

    /* factor n and n-1 to build the prime factorisation of C(n,2) */
    uint64_t primes[64];
    uint32_t exps[64];
    size_t pcnt = 0;

    /* factor n */
    {
        uint64_t m = n;
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                uint32_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                add_prime(p, e, primes, exps, &pcnt);
            }
        }
        if (m > 1) add_prime(m, 1, primes, exps, &pcnt);
    }

    /* factor n-1 */
    {
        uint64_t m = n - 1;
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                uint32_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                add_prime(p, e, primes, exps, &pcnt);
            }
        }
        if (m > 1) add_prime(m, 1, primes, exps, &pcnt);
    }

    /* remove the factor 2 from the numerator to divide by 2 in C(n,2) */
    for (size_t i = 0; i < pcnt; ++i) {
        if (primes[i] == 2) {
            if (exps[i] > 0) --exps[i];
            break;
        }
    }

    /* if C(n,2)==1 there are no constraints – all coefficients work */
    if (pcnt == 0) return (seq_t)(n + 1);

    /* Pre‑compute v_p(k!) for all required primes and k ≤ n */
    uint32_t **vp = (uint32_t **)malloc(pcnt * sizeof(uint32_t *));
    for (size_t i = 0; i < pcnt; ++i) {
        vp[i] = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
        vp[i][0] = 0;
        for (unsigned long long k = 1; k <= n; ++k) {
            unsigned long long t = k;
            uint32_t add = 0;
            while (t % primes[i] == 0) {
                t /= primes[i];
                ++add;
            }
            vp[i][k] = vp[i][k - 1] + add;
        }
    }

    /* Count suitable j */
    uint32_t result = 0;
    for (unsigned long long j = 0; j <= n; ++j) {
        int ok = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            uint32_t val = vp[i][n] - vp[i][j] - vp[i][n - j];
            if (val < exps[i]) { ok = 0; break; }
        }
        if (ok) ++result;
    }

    /* free temporary arrays */
    for (size_t i = 0; i < pcnt; ++i) free(vp[i]);
    free(vp);

    return (seq_t)result;
}
