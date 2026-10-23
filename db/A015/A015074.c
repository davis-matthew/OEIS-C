/*
 * A015074  Inverse of 1065th cyclotomic polynomial.
 * https://oeis.org/A015074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series *
 *            1/Φ1065(x). Φ1065(x) is the 1065th cyclotomic polynomial, of
 *            degree 560. * The coefficients are periodic with period 1065
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   compute_phi_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/compute_phi_coeffs.h"

seq_t a(idx_t n)
{
    /* static cache of one period */
    static struct {
        seq_t period[1065];
        int    ready;
    } cache = { .ready = 0 };

    if (!cache.ready) {
        /* compute coefficients of Φ1065(x) and the inverse series */
        seq_t phi[561];
        compute_phi_coeffs(phi);

        /* a[0] = 1, a[k] = -Sum_{i=1}^{min(k,560)} phi[i]*a[k-i]  */
        cache.period[0] = 1;
        for (size_t k = 1; k < 1065; ++k) {
            seq_t sum = 0;
            size_t upper = k < 560 ? k : 560;
            for (size_t i = 1; i <= upper; ++i)
                sum += phi[i] * cache.period[k - i];
            cache.period[k] = -sum;
        }

        cache.ready = 1;
    }
    return cache.period[(size_t)(n % 1065ULL)];
}
