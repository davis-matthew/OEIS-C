/*
 * A015890  Inverse of 1881st cyclotomic polynomial.
 * https://oeis.org/A015890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = –∑_{i=1}^{1080} c_i · a(n–i), where
 *            c_i are the coefficients of the 1881st cyclotomic polynomial
 *            Φ_1881(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   mul_x_pow_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_x_pow_minus_one.h"

/* divides a monic numerator by a monic denominator – the result is the coprime polynomial Φ_1881. */
static int64_t *poly_div(int64_t *num, int deg_num, int64_t *den, int deg_den)
{
    int deg_quo = deg_num - deg_den;
    int64_t *quo = (int64_t *)calloc((size_t)deg_quo + 1, sizeof(int64_t));
    for (int i = deg_num; i >= deg_den; --i) {
        int64_t coeff = num[i];
        int qpos = i - deg_den;
        quo[qpos] = coeff;
        for (int j = 0; j <= deg_den; ++j) {
            num[i - deg_den + j] -= coeff * den[j];
        }
    }
    free(num);
    free(den);
    return quo;
}

seq_t a(idx_t n)
{
    static seq_t period[1881];
    static bool ready = false;

    if (!ready) {
        /* divisors of 1881 = 3²·11·19 */
        static const idx_t pos[] = {9, 33, 57, 1881};    /* μ(n/d) = +1 */
        static const int   poscnt = 4;
        static const idx_t neg[] = {3, 99, 171, 627};     /* μ(n/d) = –1 */
        static const int   negcnt = 4;

        /* build numerator = Π_(d∈pos) (x^d – 1) */
        int64_t *num  = (int64_t *)calloc(1, sizeof(int64_t));
        num[0] = 1;
        int deg_num = 0;
        for (int k = 0; k < poscnt; ++k) {
            num   = mul_x_pow_minus_one(num, deg_num, (int)pos[k]);
            deg_num += pos[k];
        }

        /* build denominator = Π_(d∈neg) (x^d – 1) */
        int64_t *den  = (int64_t *)calloc(1, sizeof(int64_t));
        den[0] = 1;
        int deg_den = 0;
        for (int k = 0; k < negcnt; ++k) {
            den   = mul_x_pow_minus_one(den, deg_den, (int)neg[k]);
            deg_den += neg[k];
        }

        /* quotient gives Φ_1881(x) */
        int64_t *phi = poly_div(num, deg_num, den, deg_den);
        int deg_phi = deg_num - deg_den;               /* should be 1080 */

        /* compute the period by the recurrence */
        period[0] = 1;
        for (int idx = 1; idx < 1881; ++idx) {
            int64_t sum = 0;
            int max_i = (idx < deg_phi) ? idx : deg_phi;
            for (int i = 1; i <= max_i; ++i) {
                sum += phi[i] * (int64_t)period[idx - i];
            }
            period[idx] = -(seq_t)sum;
        }

        free(phi);
        ready = true;
    }

    int idx = (int)(n % 1881);
    return period[idx];
}
