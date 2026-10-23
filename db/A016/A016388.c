/*
 * A016388  345th cyclotomic polynomial.
 * https://oeis.org/A016388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) are the coefficients of the 345th cyclotomic polynomial
 *            Φ₃₄₅(x)
 * Range:     i8 n <= 176, i16 n <= 176, i32 n <= 176, i64 n <= 176,
 *            i128 n <= 176, big any n
 * Helpers:   mul_by_xk_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_by_xk_minus1.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t coeff[177];          /* 0 .. 176 inclusive */

    if (!init) {
        const size_t MAX_DEG = 400;   /* enough for all intermediate polynomials */
        seq_t num[MAX_DEG];
        seq_t den[MAX_DEG];
        seq_t tmp[MAX_DEG];
        size_t num_deg = 0;
        size_t den_deg = 0;

        /* start with the polynomial 1 */
        for (size_t i = 0; i < MAX_DEG; ++i) {
            num[i] = 0;
            den[i] = 0;
        }
        num[0] = 1;
        den[0] = 1;

        /* Helper macro to apply a factor (x^d - 1) with the given exponent.  */
        #define APPLY(d, e) do {                     \
            if ((e) > 0)                              \
                mul_by_xk_minus1(num, &num_deg, (d), tmp, MAX_DEG); \
            else                                       \
                mul_by_xk_minus1(den, &den_deg, (d), tmp, MAX_DEG); \
        } while (0)

        /* Exponents for the divisors of 345 are: -1, 1, 1, -1, 1, -1, -1, 1 */
        /* d = 1, 3, 5, 15, 23, 69, 115, 345 */
        APPLY(1,   -1);
        APPLY(3,    1);
        APPLY(5,    1);
        APPLY(15,  -1);
        APPLY(23,   1);
        APPLY(69,  -1);
        APPLY(115, -1);
        APPLY(345,  1);

        #undef APPLY

        /* Exact division: num / den → result coefficients */
        size_t shiftmax = num_deg - den_deg;           /* equals 176 */
        seq_t work[MAX_DEG];
        seq_t res_coef[177];                           /* maximum possible size */

        for (size_t i = 0; i <= num_deg; ++i) {
            work[i] = num[i];
        }
        for (size_t i = 0; i < 177; ++i) {
            res_coef[i] = 0;
        }

        for (long long i = (long long)shiftmax; i >= 0; --i) {
            seq_t c = work[den_deg + i];
            res_coef[i] = c;
            if (c != 0) {
                for (size_t j = 0; j <= den_deg; ++j) {
                    work[j + i] -= c * den[j];
                }
            }
        }

        /* Store result into static array */
        for (size_t i = 0; i < 177; ++i) {
            coeff[i] = 0;
        }
        for (size_t i = 0; i <= shiftmax; ++i) {
            coeff[i] = res_coef[i];
        }

        init = true;
    }

    if (n < 177) {
        return coeff[(size_t)n];
    } else {
        return 0;
    }
}
