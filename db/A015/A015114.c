/*
 * A015114  Inverse of 1105th cyclotomic polynomial.
 * https://oeis.org/A015114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n≥1, a(n) = -∑_{i=1}^{φ(1105)} φ_1105(i)·a(n‑i). The
 *            sequence is periodic with period 1105, so a(n)=a(n mod 1105)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes Φ₁₁₀₅(x) once and then uses its coefficients
   to generate the reciprocal power–series coefficients up to one period. */

#include <stdbool.h>
#include <stddef.h>

/* Exact polynomial division: out = num / den, assuming division is exact. */
static void poly_divide(const seq_t *num, int num_deg,
                        const seq_t *den, int den_deg,
                        seq_t *out, int *out_deg)
{
    int qdeg = num_deg - den_deg;
    seq_t t[1106];
    for (int i = 0; i <= num_deg; ++i) t[i] = num[i];
    for (int i = 0; i <= qdeg; ++i) out[i] = 0;     /* quotient space */
    for (int i = num_deg; i >= den_deg; --i) {
        seq_t coeff = t[i];
        int qidx = i - den_deg;
        out[qidx] = coeff;
        for (int j = 0; j <= den_deg; ++j) {
            t[i - j] -= coeff * den[den_deg - j];
        }
    }
    int d = qdeg;
    while (d > 0 && out[d] == 0) --d;
    *out_deg = d;
}

/* Period of the sequence. */
#define PERIOD 1105

/* Cached coefficients of 1/Φ₁₁₀₅(x) for indices 0..PERIOD-1. */
static seq_t seq_arr[PERIOD];
static bool seq_initialized = false;

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq_initialized) {
        /* Divisors of 1105 (=5·13·17) in increasing order. */
        const int DIVS[8] = {1, 5, 13, 17, 65, 85, 221, 1105};
        int deg[8];
        seq_t poly[8][1106];      /* coefficients of Φ_d(x) */

        /* Compute Φ_d(x) for every divisor d of 1105. */
        for (int idx = 0; idx < 8; ++idx) {
            int d = DIVS[idx];
            /* Start with (x^d−1). */
            int num_deg = d;
            seq_t num[1106];
            for (int i = 0; i <= d; ++i) num[i] = (i == d ? (seq_t)1 : (seq_t)0);
            num[0] = (seq_t)-1;    /* x^d – 1 */

            /* Divide out all Φ_e(x) for e|d and e<d. */
            for (int j = 0; j < idx; ++j) {
                int e = DIVS[j];
                if (d % e == 0) {
                    seq_t quo[1106];
                    int quo_deg;
                    poly_divide(num, num_deg,
                                poly[j], deg[j],
                                quo, &quo_deg);
                    for (int i = 0; i <= quo_deg; ++i) num[i] = quo[i];
                    num_deg = quo_deg;
                }
            }
            deg[idx] = num_deg;
            for (int i = 0; i <= num_deg; ++i)
                poly[idx][i] = num[i];
            for (int i = num_deg + 1; i <= 1105; ++i)
                poly[idx][i] = (seq_t)0;
        }

        /* Φ₁₁₀₅(x) is the last entry. */
        const seq_t *phi = poly[7];
        int phi_deg = deg[7];

        /* Compute reciprocal coefficients up to one period. */
        seq_arr[0] = (seq_t)1;
        for (int k = 1; k < PERIOD; ++k) {
            seq_t sum = (seq_t)0;
            int limit = k < phi_deg ? k : phi_deg;
            for (int i = 1; i <= limit; ++i)
                sum += phi[i] * seq_arr[k - i];
            seq_arr[k] = -sum;
        }
        seq_initialized = true;
    }

    return seq_arr[(size_t)(n % PERIOD)];
}
