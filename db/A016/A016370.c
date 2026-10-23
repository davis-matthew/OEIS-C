/*
 * A016370  273rd cyclotomic polynomial.
 * https://oeis.org/A016370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   coefficients of the 273rd cyclotomic polynomial Φ_273(x)
 * Range:     i8 n <= 144, i16 n <= 144, i32 n <= 144, i64 n <= 144,
 *            i128 n <= 144, big any n
 * Helpers:   mul_by_xm_minus_one_int32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_by_xm_minus_one_int32.h"

#define MAX_POLY 300              /* ample space for intermediate results */
#define DEG       144              /* φ(273) = 144 */

/* Static storage for the coefficients (degree 0 .. 144) */
static seq_t coeffs_static[DEG + 1];
static bool coeffs_ready = false;

/* Exact polynomial division:  numerator / denominator, where
   denominator divides numerator.  The quotient has size (num_sz - den_sz + 1). */
static void poly_divexact_int32(const int32_t *num, size_t num_sz,
                                const int32_t *den, size_t den_sz,
                                int32_t *quot, size_t q_sz)
{
    int32_t temp[MAX_POLY];
    memcpy(temp, num, num_sz * sizeof(int32_t));

    size_t deg_num = num_sz - 1;
    size_t deg_den = den_sz - 1;

    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        int32_t coeff = temp[i];          /* leading coeff of remainder */
        size_t pos = i - deg_den;         /* q index */
        quot[pos] = coeff;
        if (coeff != 0) {
            for (size_t j = 0; j < den_sz; ++j) {
                temp[pos + j] -= coeff * den[j];
            }
        }
    }
}

/* Compute all coefficients of Φ_273(x) and store them in coeffs_static */
static void compute_coeffs(void)
{
    int32_t poly_num[MAX_POLY];
    int32_t poly_den[MAX_POLY];
    int32_t tmp[MAX_POLY];
    size_t sz_num = 1;
    size_t sz_den = 1;

    /* base polynomial '1' */
    memset(poly_num, 0, sizeof(poly_num));
    memset(poly_den, 0, sizeof(poly_den));
    poly_num[0] = 1;
    poly_den[0] = 1;

    /* Construct numerator: (x^273–1)(x^13–1)(x^7–1)(x^3–1) */
    const size_t num_muls[4] = {273, 13, 7, 3};
    for (size_t k = 0; k < 4; ++k) {
        size_t m = num_muls[k];
        memset(tmp, 0, sizeof(tmp));
        mul_by_xm_minus_one_int32(tmp, poly_num, sz_num, m);
        memcpy(poly_num, tmp, (sz_num + m) * sizeof(int32_t));
        sz_num += m;
    }

    /* Construct denominator: (x^91–1)(x^39–1)(x^21–1)(x–1) */
    const size_t den_muls[4] = {91, 39, 21, 1};
    for (size_t k = 0; k < 4; ++k) {
        size_t m = den_muls[k];
        memset(tmp, 0, sizeof(tmp));
        mul_by_xm_minus_one_int32(tmp, poly_den, sz_den, m);
        memcpy(poly_den, tmp, (sz_den + m) * sizeof(int32_t));
        sz_den += m;
    }

    /* Exact division: Φ_273 = numerator / denominator */
    const size_t q_sz = sz_num - sz_den + 1;   /* 145 */
    int32_t quot[q_sz];
    poly_divexact_int32(poly_num, sz_num, poly_den, sz_den, quot, q_sz);

    /* Store result into static array, casting to seq_t */
    for (size_t i = 0; i < q_sz; ++i) {
        coeffs_static[i] = (seq_t)quot[i];
    }
    /* Pad remaining (should be none) */
    for (size_t i = q_sz; i <= DEG; ++i) {
        coeffs_static[i] = 0;
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (!coeffs_ready) {
        compute_coeffs();
        coeffs_ready = true;
    }
    if (n >= DEG + 1) return 0;
    return coeffs_static[(size_t)n];
}
