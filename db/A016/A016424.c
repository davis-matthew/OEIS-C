/*
 * A016424  495th cyclotomic polynomial.
 * https://oeis.org/A016424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 495‑th cyclotomic polynomial
 *            Φ₄₉₅(x)
 * Range:     i8 n <= 240, i16 n <= 240, i32 n <= 240, i64 n <= 240,
 *            i128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by multiplying factors (x^d−1)^1 for μ(495/d)=1 and then
   dividing by (x^d−1) for μ(495/d)=−1.  The resulting polynomial has
   degree φ(495)=240; coefficients beyond that are 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_DEG 600   /* sufficient for intermediate polynomials */

static void mul_shift_sub(const seq_t *old, size_t old_deg,
                         size_t d, seq_t *out, size_t *new_deg) {
    size_t newd = old_deg + d;
    static seq_t temp[MAX_DEG + 1];
    for (size_t i = 0; i <= newd; ++i) temp[i] = 0;
    for (size_t i = 0; i <= old_deg; ++i) {
        temp[i] -= old[i];
        temp[i + d] += old[i];
    }
    for (size_t i = 0; i <= newd; ++i) out[i] = temp[i];
    *new_deg = newd;
}

static void div_shift_sub(seq_t *poly, size_t *deg,
                          size_t d, const seq_t *result) {
    size_t cur_deg = *deg;
    static seq_t temp[MAX_DEG + 1];
    static seq_t quot[MAX_DEG + 1];
    for (size_t i = 0; i <= cur_deg; ++i) temp[i] = poly[i];
    size_t new_deg = cur_deg - d;
    for (size_t i = 0; i <= new_deg; ++i) quot[i] = 0;
    for (size_t k = cur_deg; ; --k) {
        seq_t coeff = temp[k];
        if (coeff != 0) {
            quot[k - d] = coeff;
            temp[k - d] += coeff;   /* subtract coeff * (x^d-1) */
        }
        if (k == d) break;          /* avoid unsigned underflow */
    }
    for (size_t i = 0; i <= new_deg; ++i) poly[i] = quot[i];
    *deg = new_deg;
}

/* Build the coefficients of Φ_495 (degree 240) */
static void build_495_coeffs(seq_t *out) {
    seq_t poly[MAX_DEG + 1];
    size_t deg = 0;
    poly[0] = 1;
    /* Multiplication factors: μ(495/d)=+1 */
    const size_t mul_d[] = {9, 15, 33, 495};
    const size_t mul_cnt = sizeof(mul_d) / sizeof(mul_d[0]);
    for (size_t i = 0; i < mul_cnt; ++i)
        mul_shift_sub(poly, deg, mul_d[i], poly, &deg);
    /* Division factors: μ(495/d)=−1 */
    const size_t div_d[] = {3, 45, 99, 165};
    const size_t div_cnt = sizeof(div_d) / sizeof(div_d[0]);
    for (size_t i = 0; i < div_cnt; ++i)
        div_shift_sub(poly, &deg, div_d[i], poly);
    /* Copy to output (length 241, degrees 0..240) */
    for (size_t i = 0; i <= deg; ++i) out[i] = poly[i];
    for (size_t i = deg + 1; i < 241; ++i) out[i] = 0;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t coeffs[241];
    if (!initialized) {
        build_495_coeffs(coeffs);
        initialized = true;
    }
    if (n >= 241) return 0;
    return coeffs[n];
}
