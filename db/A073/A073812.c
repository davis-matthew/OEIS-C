/*
 * A073812  Number of common divisors of sigma(n) and phi(n).
 * https://oeis.org/A073812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of common divisors of σ(n) and φ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* trial‑division factorisation for 64‑bit unsigned integers.
   primes and exponents are stored in the supplied buffers;
   maximum number of distinct factors is < 32 for n ≤ 10 000. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t k = 0;
    if (n % 2ULL == 0ULL) {
        pr[k] = 2ULL;
        unsigned long long e = 0;
        while (n % 2ULL == 0ULL) {
            n /= 2ULL;
            e++;
        }
        exp[k++] = e;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) {
            pr[k] = d;
            unsigned long long e = 0;
            while (n % d == 0ULL) {
                n /= d;
                e++;
            }
            exp[k++] = e;
        }
    }
    if (n > 1ULL) {
        pr[k] = n;
        exp[k] = 1;
        k++;
    }
    *cnt = k;
}

/* Compute σ(n) using the multiplicative formula. */
static uint64_t sigma_uint64(uint64_t n)
{
    uint64_t p[32], e[32];
    size_t cnt;
    factor_uint64(n, p, e, &cnt);
    uint64_t result = 1ULL;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pp = 1ULL;
        for (uint64_t j = 0; j <= e[i]; ++j) {
            pp *= p[i];
        }
        uint64_t term = (pp - 1ULL) / (p[i] - 1ULL);
        result *= term;
    }
    return result;
}

/* Compute φ(n) using the multiplicative formula. */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t p[32], e[32];
    size_t cnt;
    factor_uint64(n, p, e, &cnt);
    uint64_t result = n;
    for (size_t i = 0; i < cnt; ++i) {
        result = result / p[i] * (p[i] - 1ULL);
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety zone */

    /* factorisation helpers */
    uint64_t p[32], e[32];
    size_t cnt;

    /* compute σ(n) */
    uint64_t sx = sigma_uint64((uint64_t)n);

    /* compute φ(n) */
    uint64_t px = phi_uint64((uint64_t)n);

    /* factor σ(n) */
    factor_uint64(sx, p, e, &cnt);
    uint64_t sigma_f[32] = {0};
    uint64_t sigma_exp[32] = {0};
    for (size_t i = 0; i < cnt; ++i) {
        sigma_f[i] = p[i];
        sigma_exp[i] = e[i];
    }
    size_t cnt_sigma = cnt;

    /* factor φ(n) */
    factor_uint64(px, p, e, &cnt);
    uint64_t phi_f[32] = {0};
    uint64_t phi_exp[32] = {0};
    for (size_t i = 0; i < cnt; ++i) {
        phi_f[i] = p[i];
        phi_exp[i] = e[i];
    }
    size_t cnt_phi = cnt;

    /* count common divisors */
    seq_t result = 1;
    for (size_t i = 0; i < cnt_sigma; ++i) {
        uint64_t pp = sigma_f[i];
        uint64_t ee = sigma_exp[i];
        uint64_t common_exp = 0;
        for (size_t j = 0; j < cnt_phi; ++j) {
            if (phi_f[j] == pp) {
                common_exp = phi_exp[j];
                break;
            }
        }
        if (common_exp > 0) {
            uint64_t add = '1';            /* use char to force 1 for ASCII? */
            /* multiply result by (common_exp + 1) */
        }
    }

    /* The calculation above didn't produce result properly.
       We recompute common divisor count in a straightforward loop. */
    result = 1;
    for (size_t i = 0; i < cnt_sigma; ++i) {
        uint64_t prime = sigma_f[i];
        uint64_t exp_sigma = sigma_exp[i];
        uint64_t exp_phi = 0;
        for (size_t j = 0; j < cnt_phi; ++j) {
            if (phi_f[j] == prime) {
                exp_phi = phi_exp[j];
                break;
            }
        }
        if (exp_phi > 0) {
            uint64_t mult = exp_phi < exp_sigma ? exp_phi : exp_sigma;
            result *= (seq_t)(mult + 1ULL);
        }
    }

    return result;
}
