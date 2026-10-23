/*
 * A014757  Inverse of 748th cyclotomic polynomial.
 * https://oeis.org/A014757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n-th coefficient of the power series expansion of
 *            1/Φ_748(x) (inverse of the 748th cyclotomic polynomial)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Multiply poly by (x^k - 1).  The array size is set to cover all
   possible degrees after multiplication: for Φ_748 the maximal degree
   is 808.  The result is written back into poly. */
static void mul_xk_minus_one(seq_t *poly, size_t *deg, int k)
{
    /* maximum possible degree of Φ_748 = [748 + 34 + 22 + 4] = 808 */
    static seq_t tmp[809];
    size_t old_deg = *deg;
    size_t new_deg = old_deg + (size_t)k;
    for (size_t i = 0; i <= new_deg; ++i)
        tmp[i] = 0;
    for (size_t i = 0; i <= old_deg; ++i) {
        tmp[i] -= poly[i];          /* -1 * poly[i]   */
        tmp[i + k] += poly[i];      /* + x^k * poly[i] */
    }
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = tmp[i];
    *deg = new_deg;
}

seq_t a(idx_t n)
{
    /* memoised coefficients of the inverse series (period 748) */
    static bool init = false;
    static seq_t cache[748];

    if (!init) {
        /* Build the numerator N = Π_{μ=1} (x^k - 1)   (k = 748/d, μ(d) = +1) */
        /* Build the denominator D = Π_{μ=-1} (x^k - 1) (k = 748/d, μ(d) = -1) */
        const int pos_plus[]  = { 748, 34, 22, 4 };   /* μ = +1 */
        const int pos_minus[] = { 374, 68, 44, 2 };   /* μ = -1 */

        /* arrays large enough for degrees up to 808 */
        static seq_t N[809];
        static seq_t D[809];
        size_t N_deg = 0, D_deg = 0;

        N[0] = 1;
        D[0] = 1;

        /* numerator */
        for (size_t i = 0; i < sizeof(pos_plus) / sizeof(pos_plus[0]); ++i)
            mul_xk_minus_one(N, &N_deg, pos_plus[i]);

        /* denominator */
        for (size_t i = 0; i < sizeof(pos_minus) / sizeof(pos_minus[0]); ++i)
            mul_xk_minus_one(D, &D_deg, pos_minus[i]);

        /* Coefficients of the inverse series up to degree 747 */
        static seq_t Ncoeff[748];
        static seq_t Dcoeff[748];
        static seq_t A[748];

        for (size_t i = 0; i < 748; ++i) {
            Ncoeff[i] = (i <= N_deg) ? N[i] : 0;
            Dcoeff[i] = (i <= D_deg) ? D[i] : 0;
        }

        /* A[0] = 1 because N[0] = D[0] = 1 */
        A[0] = 1;
        for (size_t i = 1; i < 748; ++i) {
            seq_t sum = Dcoeff[i];
            for (size_t k = 1; k <= i; ++k)
                sum -= Ncoeff[k] * A[i - k];
            A[i] = sum;   /* N[0] == 1, so no division needed */
        }

        /* store into cache for fast lookup */
        for (size_t i = 0; i < 748; ++i)
            cache[i] = A[i];

        init = true;
    }

    size_t pos = (size_t)(n % 748ULL);
    return cache[pos];
}
