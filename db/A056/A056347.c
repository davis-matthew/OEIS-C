/*
 * A056347  Number of primitive (period n) bracelets using a maximum of six
 *          different colored beads.
 * https://oeis.org/A056347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * B(n/d) with B(m) = (1/(2m)) · ( Σ_{d|m}
 *            φ(m/d)·6^d + (m odd) m·6^((m+1)/2) | (m even)
 *            (m/2)·(6^(m/2)+6^(m/2+1)) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mu_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mu_func.h"

/* ---------- fast exponentiation for seq_t ---------- */
static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- Euler totient function ---------- */
static seq_t phi_seq(idx_t n)
{
    seq_t result = n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* ---------- number of (not necessarily primitive) bracelets with 6 colors ---------- */
static seq_t B_func(idx_t m)
{
    const seq_t six = 6;
    seq_t S = 0;

    /* sum φ(m/d)·6^d over all divisors d of m */
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            seq_t term1 = phi_seq(m / d1) * pow_big(six, d1);
            S += term1;

            idx_t d2 = m / i;
            if (d2 != d1) {
                seq_t term2 = phi_seq(m / d2) * pow_big(six, d2);
                S += term2;
            }
        }
    }

    seq_t B;
    if (m % 2 == 1) {                       /* m is odd */
        seq_t part = (seq_t)m * pow_big(six, (m + 1) / 2);
        B = (S + part) / ((seq_t)(2 * m));
    } else {                                /* m is even */
        seq_t half = (seq_t)(m / 2);
        seq_t part = half * (pow_big(six, m / 2) + pow_big(six, m / 2 + 1));
        B = (S + part) / ((seq_t)(2 * m));
    }
    return B;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            int mu1 = mu_func(d1);
            if (mu1 != 0) {
                seq_t B = B_func(n / d1);
                if (mu1 > 0) sum_pos += B; else sum_neg += B;
            }

            idx_t d2 = n / i;
            if (d2 != d1) {
                int mu2 = mu_func(d2);
                if (mu2 != 0) {
                    seq_t B = B_func(n / d2);
                    if (mu2 > 0) sum_pos += B; else sum_neg += B;
                }
            }
        }
    }

    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
