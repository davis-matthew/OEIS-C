/*
 * A014240  Inverse of 231st cyclotomic polynomial.
 * https://oeis.org/A014240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0: a(n) = -∑_{k=1}^{120} c_k·a(n‑k), where c_k are coefficients of Φ₍₂₃₁₎(x). The sequence is periodic with period 231. */
#include <stddef.h>

static seq_t coeff[121];
static seq_t period[231];
static int init_done = 0;

/* Build Φ₍₂₃₁₎(x) = (x³-1)(x⁷-1)(x¹¹-1)(x²³¹-1)
/                                 -------------------
                                  (x-1)(x²¹-1)(x³³-1)(x⁷⁷-1) */
static void compute_phi_coeffs(void)
{
    /* maximum degree after multiplications is 252; use a fixed-size buffer */
    const size_t BUFFER = 260;
    seq_t num[BUFFER];
    seq_t den[BUFFER];
    seq_t tmp[BUFFER];
    size_t num_len = 1, den_len = 1;
    num[0] = 1;
    den[0] = 1;
    /* numerator factors d = 3,7,11,231 */
    const size_t num_factors[] = {3, 7, 11, 231};
    for (size_t f = 0; f < 4; ++f) {
        size_t d = num_factors[f];
        size_t new_len = num_len + d;
        for (size_t j = 0; j <= new_len; ++j) tmp[j] = 0;
        for (size_t j = 0; j < new_len; ++j) {
            seq_t val = 0;
            if (j < num_len) val += num[j];
            if (j >= d && j - d < num_len) val -= num[j - d];
            tmp[j] = val;
        }
        for (size_t j = 0; j < new_len; ++j) num[j] = tmp[j];
        num_len = new_len;
    }
    /* denominator factors d = 1,21,33,77 */
    const size_t den_factors[] = {1, 21, 33, 77};
    for (size_t f = 0; f < 4; ++f) {
        size_t d = den_factors[f];
        size_t new_len = den_len + d;
        for (size_t j = 0; j <= new_len; ++j) tmp[j] = 0;
        for (size_t j = 0; j < new_len; ++j) {
            seq_t val = 0;
            if (j < den_len) val += den[j];
            if (j >= d && j - d < den_len) val -= den[j - d];
            tmp[j] = val;
        }
        for (size_t j = 0; j < new_len; ++j) den[j] = tmp[j];
        den_len = new_len;
    }
    /* long division : num / den = Φ₍₂₃₁₎ */
    size_t deg_num = num_len - 1;
    size_t deg_den = den_len - 1;
    size_t deg_q   = deg_num - deg_den;   /* should be 120 */
    seq_t quot[BUFFER];
    for (size_t k = 0; k <= deg_q; ++k) quot[k] = 0;
    /* division */
    for (long long i = (long long)deg_num; i >= (long long)deg_den; --i) {
        seq_t c = num[i];
        size_t t = (size_t)(i - deg_den);
        quot[t] = c;
        for (size_t j = 0; j <= deg_den; ++j) {
            num[t + j] -= c * den[j];
        }
    }
    /* copy coefficients to global array */
    for (size_t k = 0; k <= deg_q; ++k) coeff[k] = quot[k];
}

/* Initialise the 231‑term periodic array */
static void init_period_terms(void)
{
    compute_phi_coeffs();
    period[0] = 1;
    for (idx_t n = 1; n < 231; ++n) {
        seq_t sum = 0;
        idx_t kmax = (n < 120) ? n : 120;
        for (idx_t k = 1; k <= kmax; ++k) {
            sum += coeff[k] * period[n - k];
        }
        period[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        init_period_terms();
        init_done = 1;
    }
    return period[(size_t)n % 231];
}
