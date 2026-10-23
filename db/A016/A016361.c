/*
 * A016361  230th cyclotomic polynomial.
 * https://oeis.org/A016361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 230th cyclotomic polynomial
 *            Φ₍₂₃₀₎(x)
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* The table of coefficient of Φ₂₃₀(x) up to its degree (φ(230)=88).
 * We compute it lazily at the first call and cache the result.
 */
static seq_t coeffs[89];
static bool initialized = false;

/* Multiply a polynomial by (x^k - 1).
 * The polynomial is given by coeff array of length *len.
 * The result is stored back in coeff, and *len is updated.
 */
static void mul_by_xk_minus1(seq_t *coeff, size_t *len, uint16_t k)
{
    size_t new_len = *len + k;
    /* The maximum degree we anticipate is 260; use a local array. */
    seq_t new_coeff[300] = {0};
    for (size_t i = 0; i < *len; ++i) {
        new_coeff[i] -= coeff[i];
        new_coeff[i + k] += coeff[i];
    }
    /* Copy back */
    for (size_t i = 0; i < new_len; ++i)
        coeff[i] = new_coeff[i];
    *len = new_len;
}

/* Compute Φ₂₃₀(x) coefficients once. */
static void compute_coeffs(void)
{
    /* Numerator factors (x^k - 1)^[+1] : k = 2, 5, 23, 230 */
    const uint16_t num_k[] = {2, 5, 23, 230};
    /* Denominator factors (x^k - 1)^[−1] : k = 1, 10, 46, 115 */
    const uint16_t den_k[] = {1, 10, 46, 115};

    /* Build numerator polynomial */
    seq_t num[300] = {0};
    size_t len_num = 1;      /* start with polynomial 1 */
    num[0] = 1;
    for (size_t i = 0; i < sizeof(num_k)/sizeof(num_k[0]); ++i)
        mul_by_xk_minus1(num, &len_num, num_k[i]);

    /* Build denominator polynomial */
    seq_t den[300] = {0};
    size_t len_den = 1;      /* start with polynomial 1 */
    den[0] = 1;
    for (size_t i = 0; i < sizeof(den_k)/sizeof(den_k[0]); ++i)
        mul_by_xk_minus1(den, &len_den, den_k[i]);

    /* Perform exact division num / den.
     * The leading coefficient of den is 1, so division is simple. */
    int deg_num = (int)len_num - 1;
    int deg_den = (int)len_den - 1;
    int deg_res = deg_num - deg_den;      /* should be 88 */
    seq_t res[90] = {0};                  /* 89 coefficients: 0..88 */

    for (int i = deg_num; i >= deg_den; --i) {
        seq_t coeff = num[i];
        if (coeff != 0) {
            int shift = i - deg_den;
            res[shift] = coeff;
            /* subtract coeff * den shifted by 'shift' */
            for (int j = 0; j <= deg_den; ++j) {
                num[shift + j] -= coeff * den[j];
            }
        }
    }

    /* Store the result in the static array */
    for (int i = 0; i <= deg_res; ++i)
        coeffs[i] = res[i];
}

/* Public interface */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_coeffs();
        initialized = true;
    }
    if (n < (idx_t)89) {
        return coeffs[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
