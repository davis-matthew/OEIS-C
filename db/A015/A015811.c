/*
 * A015811  Inverse of 1802nd cyclotomic polynomial.
 * https://oeis.org/A015811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power‑series expansion of
 *            1/Φ_1802(x), where Φ_1802(x) is the 1802nd cyclotomic
 *            polynomial. The sequence is periodic with period 1802. The
 *            implementation computes the polynomial Φ_1802(x) via its product
 *            representation and then uses a linear recurrence to obt...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1896 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/mem.h"

/* exact polynomial division:  num / den.  num and den are freed by this
   function.  The result is a polynomial of length q_len.  */
static seq_t *poly_division(seq_t *num, size_t n_len,
                            seq_t *den, size_t d_len,
                            size_t *q_len)
{
    /* Make a copy of the remainder */
    seq_t *rem = mem_new(seq_t, n_len);
    for (size_t i = 0; i < n_len; ++i) rem[i] = num[i];

    *q_len = n_len - d_len + 1;
    seq_t *q = mem_new(seq_t, *q_len);

    for (int i = (int)n_len - 1; i >= (int)d_len - 1; --i) {
        seq_t coeff = rem[i];          /* leading term of remainder */
        if (coeff != 0) {
            size_t shift = (size_t)(i - (d_len - 1));
            q[shift] = coeff;
            for (size_t j = 0; j < d_len; ++j) {
                rem[shift + j] -= coeff * den[j];
            }
        }
    }

    mem_free(rem);
    mem_free(num);
    mem_free(den);
    return q;
}

/* multiply a polynomial by (x^k - 1)  */
static seq_t *poly_mul_xk_minus_one(seq_t *poly, size_t len, size_t k)
{
    size_t new_len = len + k;
    seq_t *new_poly = mem_new(seq_t, new_len);
    for (size_t i = 0; i < len; ++i) {
        new_poly[i] -= poly[i];
        new_poly[i + k] += poly[i];
    }
    mem_free(poly);
    return new_poly;
}

/* Compute the first period of the sequence.  The result is stored in a
   static array and returned to the caller.  */
static seq_t *prepare_sequence(void)
{
    /* 1. Build Φ_1802(x) through its product representation.  */
    /*    Positive‑exponent factors (μ = +1)  */
    const size_t pos_k[] = {1802, 53, 17, 2};
    const size_t num_pos = sizeof(pos_k) / sizeof(pos_k[0]);

    /*    Negative‑exponent factors (μ = -1)  */
    const size_t neg_k[] = {901, 106, 34, 1};
    const size_t num_neg = sizeof(neg_k) / sizeof(neg_k[0]);

    /* Numerator: product of (x^k - 1) for k in pos_k */
    size_t num_len = 1;
    seq_t *num = mem_new(seq_t, 1);
    num[0] = 1;
    for (size_t i = 0; i < num_pos; ++i) {
        num = poly_mul_xk_minus_one(num, num_len, pos_k[i]);
        num_len += pos_k[i];
    }

    /* Denominator: product of (x^k - 1) for k in neg_k */
    size_t den_len = 1;
    seq_t *den = mem_new(seq_t, 1);
    den[0] = 1;
    for (size_t i = 0; i < num_neg; ++i) {
        den = poly_mul_xk_minus_one(den, den_len, neg_k[i]);
        den_len += neg_k[i];
    }

    /* Perform exact division to obtain Φ_1802(x) */
    size_t phi_len;
    seq_t *phi = poly_division(num, num_len, den, den_len, &phi_len);
    /* phi_len should be 833 (degree 832 + 1 coefficients) */
    assert(phi_len == 833);

    /* 2. Compute coefficients a[0..1801] by the linear recurrence. */
    const size_t PERIOD = 1802;
    seq_t *a = mem_new(seq_t, PERIOD);
    a[0] = 1;                               /* constant term of 1/Φ */

    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t maxk = (n < (phi_len - 1)) ? n : (phi_len - 1);
        for (size_t k = 1; k <= maxk; ++k) {
            sum += phi[k] * a[n - k];
        }
        a[n] = -sum;
    }

    mem_free(phi);
    return a;
}

/* Public function required by the build.  It returns the nth term of
   the sequence, using periodicity.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    if (cache == NULL) {
        cache = prepare_sequence();
    }
    const size_t PERIOD = 1802;
    return cache[(size_t)(n % PERIOD)];
}
