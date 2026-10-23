/*
 * A145394  Number of inequivalent sublattices of index n in hexagonal
 *          lattice, where two sublattices are considered equivalent if one
 *          can be rotated by a multiple of Pi/3 to give the other.
 * https://oeis.org/A145394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (σ(n) + 2 * Σ_{d|n} χ(d)) / 3 where σ(n) = sum of
 *            divisors of n, χ(d) = Dirichlet character modulo 3: χ(d)= 0 if
 *            3|d, χ(d)= 1 if d ≡ 1 (mod 3), χ(d)= –1 if d ≡ 2 (mod 3). The
 *            expression is integer for all n ≥ 1
 * Range:     u8 n <= 16, u16 n <= 1164, u32 n <= 7977, u64 n <= 7977,
 *            u128 n <= 7977, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    uint64_t N = n;                 /* working copy of n */
    seq_t sigma = 1;                /* σ(n) */
    seq_t chi_sum = 1;              /* Σ_{d|n} χ(d) */

    /* factor 2 */
    unsigned e2 = 0;
    while ((N & 1ULL) == 0) { N >>= 1; e2++; }
    if (e2) {
        seq_t pwr = pow_uint(2, e2 + 1);
        sigma *= (pwr - 1);          /* 2^(e2+1) – 1 */
        if (chi_sum && (e2 & 1U))
            chi_sum = 0;             /* odd exponent makes χ_sum zero */
    }

    /* factor 3 */
    unsigned e3 = 0;
    while (N % 3ULL == 0) { N /= 3; e3++; }
    if (e3) {
        seq_t pwr = pow_uint(3, e3 + 1);
        sigma *= (pwr - 1);
        sigma /= 2;                  /* divide by (3‑1) */
        /* χ(3)=0, but any power still contributes 1 to the product */
    }

    /* remaining odd prime factors >3 */
    for (uint64_t p = 5; p * p <= N; p += 2) {
        if (N % p == 0) {
            unsigned e = 0;
            while (N % p == 0) { N /= p; e++; }
            seq_t pwr = pow_uint(p, e + 1);
            sigma *= (pwr - 1);
            sigma /= (p - 1);
            if (chi_sum) {
                if (p % 3ULL == 1) {
                    chi_sum *= (e + 1);
                } else { /* p ≡ 2 (mod 3) */
                    if (e & 1U)
                        chi_sum = 0;   /* odd exponent => zero contribution */
                }
            }
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (N > 1) {
        seq_t pwr = pow_uint(N, 2);   /* N^(1+1) */
        sigma *= (pwr - 1);
        sigma /= (N - 1);
        if (chi_sum) {
            if (N % 3ULL == 1) {
                chi_sum *= 2;        /* e = 1 => e+1 = 2 */
            } else if (N % 3ULL == 2) {
                chi_sum = 0;
            }
        }
    }

    /* final formula */
    seq_t numerator = sigma + chi_sum * 2;
    seq_t result = numerator / 3;
    return result;
}
