/*
 * A023142  Number of cycles of function f(x) = 10x mod n.
 * https://oeis.org/A023142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|m} φ(d)/ord_{10}(d), where m = n with all 2 and 5
 *            factors removed
 * Range:     u8 n <= 998, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/powmod64.h"

/* Compute multiplicative order of 10 modulo d (d>1).  For d==1 returns 1. */
static seq_t ord10(uint64_t d)
{
    if (d == 1) return (seq_t)1;
    /* phi(d) */
    seq_t phi_seq = calc_phi((seq_t)d);
    uint64_t phi = (uint64_t)phi_seq;
    /* factor phi */
    uint64_t primes[20], exps[20];
    size_t pcnt = 0;
    factor_u64(phi, primes, exps, &pcnt);
    uint64_t ord = phi;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        while (ord % p == 0) {
            uint64_t candidate = ord / p;
            if (powmod64(10, candidate, d) == 1ULL)
                ord = candidate;
            else
                break;
        }
    }
    return (seq_t)ord;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    /* Remove all 2s and 5s */
    while ((m & 1ULL) == 0) m >>= 1;
    while (m % 5ULL == 0) m /= 5;
    if (m == 0) m = 1;           /* shouldn't happen for n>=1 */
    /* Factor m */
    uint64_t primes[20], exps[20];
    size_t pcnt = 0;
    factor_u64(m, primes, exps, &pcnt);
    /* Compute total number of divisors */
    size_t total = 1;
    for (size_t i = 0; i < pcnt; ++i) total *= (exps[i] + 1);
    /* Generate all divisors */
    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!divs) return 0;
    size_t divcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t prev_cnt = divcnt;
        uint64_t mul = 1;
        for (unsigned e = 1; e <= exps[i]; ++e) {
            mul *= p;
            for (size_t j = 0; j < prev_cnt; ++j)
                divs[divcnt++] = divs[j] * mul;
        }
    }
    /* Sum over divisors */
    seq_t result = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        seq_t phi_d = calc_phi((seq_t)d);
        seq_t ord_d = ord10(d);
        result += phi_d / ord_d;
    }
    free(divs);
    return result;
}
