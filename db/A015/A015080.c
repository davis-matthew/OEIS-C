/*
 * A015080  Inverse of 1071st cyclotomic polynomial.
 * https://oeis.org/A015080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence based on the inverse of the 1071st cyclotomic polynomial Φ_1071(x) :
 *
 *   a(0) = 1
 *   for n ≥ 1
 *     a(n) = - Σ_{k=1}^{min(n,τ)} c_k · a(n‑k)
 *   where c_k are the coefficients of Φ_1071(x) (c_0 = 1) and τ is its degree.
 *
 * The coefficients of Φ_1071(x) are computed once using the product
 *   Φ_n(x) = ∏_{d|n} (x^{n/d} – 1)^{μ(d)},
 * where μ is the Möbius function.
 *
 * The resulting sequence is periodic with period 1071.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PERIOD 1071

static bool coeffs_ready = false;
static seq_t coeffs[577];      /* Φ_1071(x) has degree 576 */
static seq_t seq_cache[PERIOD];

/* Compute Φ_1071(x) coefficients and the first PERIOD terms of the sequence. */
static void compute_coeffs_and_seq(void)
{
    /* Möbius-positive divisors: d = 1,21,51,119  →  shift = n/d = 1071,51,21,9  */
    const idx_t mult_shifts[] = {9, 21, 51, 1071};
    const size_t mult_cnt = sizeof(mult_shifts) / sizeof(mult_shifts[0]);

    /* Möbius-negative divisors: d = 3,7,17   →  shift = 357,153,63,3  */
    const idx_t div_shifts[] = {357, 153, 63, 3};
    const size_t div_cnt = sizeof(div_shifts) / sizeof(div_shifts[0]);

    /* Temporary polynomials – size ample for degree 576 */
    seq_t poly[2000]   = {0};
    seq_t newpoly[2000] = {0};
    size_t poly_len = 1;
    poly[0] = 1;

    /* Multiply by (x^shift – 1) for μ(d) = +1 divisors */
    for (size_t i = 0; i < mult_cnt; ++i) {
        size_t shift = (size_t)mult_shifts[i];
        size_t new_len = poly_len + shift;
        for (size_t j = 0; j < new_len; ++j) newpoly[j] = 0;
        for (size_t j = 0; j < poly_len; ++j) {
            newpoly[j]          -= poly[j];
            newpoly[j + shift]  += poly[j];
        }
        for (size_t j = 0; j < new_len; ++j) poly[j] = newpoly[j];
        poly_len = new_len;
    }

    /* Divide by (x^shift – 1) for μ(d) = –1 divisors */
    for (size_t i = 0; i < div_cnt; ++i) {
        size_t shift = (size_t)div_shifts[i];
        size_t new_len = poly_len - shift;
        for (size_t j = 0; j < new_len; ++j) {
            if (j < shift) {
                newpoly[j] = -poly[j];
            } else {
                newpoly[j] = newpoly[j - shift] - poly[j];
            }
        }
        for (size_t j = 0; j < new_len; ++j) poly[j] = newpoly[j];
        poly_len = new_len;
    }

    /* Store Φ_1071(x) coefficients (c_0 … c_τ) */
    for (size_t i = 0; i < poly_len; ++i) coeffs[i] = poly[i];

    /* Compute the first PERIOD terms of the sequence by the recurrence */
    seq_cache[0] = 1;
    size_t deg = poly_len - 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t m = (n < deg) ? n : deg;
        for (size_t k = 1; k <= m; ++k) {
            sum += -coeffs[k] * seq_cache[n - k];
        }
        seq_cache[n] = sum;
    }
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (!coeffs_ready) {
        compute_coeffs_and_seq();
        coeffs_ready = true;
    }
    return seq_cache[n % PERIOD];
}
