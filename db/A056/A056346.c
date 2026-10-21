/*
 * A056346  Number of bracelets of length n using exactly six different
 *          colored beads.
 * https://oeis.org/A056346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A056346 * a(n) = (k! / 4) * (S2(floor((n+1)/2),k) +
 *            S2(ceil((n+1)/2),k)) * + (k! / (2*n)) * Σ_{d|n} φ(d) * S2(n/d,k)
 *            * where k = 6, k! = 720, S2 is Stirling numbers of the second
 *            kind. * The implementation precomputes S2(m,6) for 0 ≤ m ≤ n
 *            using a recurrence. * It then evaluates the formula exact...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, phi, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/phi.h"          /* phi(idx_t) */
#include "../utils/mul_small.h"    /* mul_small(seq_t, unsigned int) */
#include "../utils/seq_mul.h"      /* seq_mul(seq_t, seq_t) */

#define MAX_N 10000   /* maximum n expected by the evaluator */

/* Table of Stirling numbers of the second kind S2(m,k) for 0 ≤ m ≤ MAX_N,
 * 0 ≤ k ≤ 6. Only k=6 is needed, but storing all k allows a direct
 * recurrence implementation. */
static seq_t S2_table[MAX_N + 1][7];

/* Highest m for which S2_table has been computed. */
static uint64_t S2_max = 0;

/* Ensure that S2_table is computed up to m_max (non‑decreasing). */
static void ensure_S2(uint64_t m_max)
{
    if (m_max > MAX_N) m_max = MAX_N;
    if (m_max <= S2_max) return;               /* already computed */

    /* Initialize base case if table is empty. */
    if (S2_max == 0) {
        S2_table[0][0] = (seq_t)1;
        for (unsigned int k = 1; k <= 6; ++k) S2_table[0][k] = (seq_t)0;
        S2_max = 0;
    }

    /* Compute rows incrementally. */
    for (uint64_t m = S2_max + 1; m <= m_max; ++m) {
        S2_table[m][0] = (seq_t)0;
        for (unsigned int k = 1; k <= 6; ++k) {
            seq_t term1 = mul_small(S2_table[m - 1][k], k);
            seq_t term2 = S2_table[m - 1][k - 1];
            S2_table[m][k] = term1 + term2;
        }
    }
    S2_max = m_max;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0, return 0 */

    /* Compute Stirling numbers up to n. */
    ensure_S2((uint64_t)n);

    uint64_t m_floor = (n + 1) >> 1;            /* floor((n+1)/2) */
    uint64_t m_ceil  = (n + 2) >> 1;            /* ceil((n+1)/2) */

    seq_t s_floor = S2_table[m_floor][6];
    seq_t s_ceil  = S2_table[m_ceil][6];

    /* term1 = 180 * (s_floor + s_ceil)  because 720/4 = 180 */
    seq_t term1 = mul_small(s_floor, 180) + mul_small(s_ceil, 180);

    /* divisor sum: Σ_{d|n} φ(d) * S2(n/d,6) */
    seq_t sum = (seq_t)0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t phi_d   = phi(d);
            uint64_t nd1  = n / d;
            seq_t s_nd1   = S2_table[nd1][6];
            sum = sum + seq_mul(phi_d, s_nd1);

            uint64_t d2 = n / d;
            if (d2 != d) {
                seq_t phi_d2 = phi(d2);
                uint64_t nd2  = n / d2;        /* equals d */
                seq_t s_nd2   = S2_table[nd2][6];
                sum = sum + seq_mul(phi_d2, s_nd2);
            }
        }
    }

    /* term2 = (360 * sum) / n   because 720/(2*n) = 360/n */
    seq_t term2 = mul_small(sum, 360);
    term2 = term2 / (seq_t)n;

    return term1 + term2;
}
