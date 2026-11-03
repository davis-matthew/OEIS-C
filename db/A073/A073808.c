/*
 * A073808  Number of common divisors of sigma_1(n) and sigma_2(n).
 * https://oeis.org/A073808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of common divisors of sigma_1(n) and sigma_2(n)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 120 terms through n = 9583 (2026-11-01)
 * Helpers:   gcd_u64, ipow_u64_u32_to_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/ipow_u64_u32_to_u64.h"
#include "../utils/num_divisors.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* ---------- Compute sigma_1(n) and sigma_2(n) -------------------------------- */
static void _sigma1_sigma2(uint64_t n, uint64_t *s1, uint64_t *s2)
{
    uint64_t pr[20], exp[20];
    size_t cnt;
    _factor_u64(n, pr, exp, &cnt);

    *s1 = 1;
    *s2 = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        unsigned e = (unsigned)exp[i];

        /* sigma_1 term: (p^(e+1)-1)/(p-1) */
        uint64_t p_pow_e1 = ipow_u64_u32_to_u64(p, e + 1);
        uint64_t term1 = (p_pow_e1 - 1) / (p - 1);
        *s1 *= term1;

        /* sigma_2 term: (p^(2e+2)-1)/(p^2-1) */
        uint64_t p_pow_2e2 = ipow_u64_u32_to_u64(p, 2 * e + 2);
        uint64_t den2 = p * p - 1;
        uint64_t term2 = (p_pow_2e2 - 1) / den2;
        *s2 *= term2;
    }
}

/* ---------- Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    uint64_t sigma1, sigma2;
    _sigma1_sigma2((uint64_t)n, &sigma1, &sigma2);

    uint64_t g = gcd_u64(sigma1, sigma2);
    uint64_t dcnt = num_divisors(g);

    return (seq_t) dcnt;
}
