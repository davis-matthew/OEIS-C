/*
 * A016374  287th cyclotomic polynomial.
 * https://oeis.org/A016374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 240, i16 n <= 240, i32 n <= 240, i64 n <= 240,
 *            i128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in the 287th cyclotomic polynomial Φ_287(x) */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t quotient[241];
    static int initialized = 0;

    if (!initialized) {
        /* constants */
        const size_t N            = 287;              /* 7*41 */
        const size_t p            = 7;
        const size_t q            = 41;
        const size_t dividend_len = N + 1;            /* 288 */
        const size_t temp_len     = p + q - 1;        /* 47   */
        const size_t divisor_len  = temp_len + 1;     /* 48   */

        /* dividend: x^N - 1  ->  coeff[0] = -1, coeff[N] = 1 */
        seq_t dividend[288];
        for (size_t i = 0; i < dividend_len; ++i) dividend[i] = 0;
        dividend[0] = (seq_t)-1;
        dividend[N] = 1;

        /* temp = Φ_p * Φ_q  (coefficients all 1 up to degree p-1 and q-1) */
        seq_t temp[47];
        for (size_t i = 0; i < temp_len; ++i) temp[i] = 0;
        for (size_t i = 0; i < p; ++i)
            for (size_t j = 0; j < q; ++j)
                temp[i + j] += 1;

        /* divisor = (x-1) * temp  ⇔  divisor[k] = temp[k-1] - temp[k] */
        seq_t divisor[48];
        for (size_t k = 0; k < divisor_len; ++k) {
            seq_t left  = (k == 0) ? 0 : temp[k - 1];
            seq_t right = (k == temp_len) ? 0 : temp[k];
            divisor[k] = left - right;
        }

        /* long division: dividend / divisor → quotient (degree 240) */
        for (int i = (int)dividend_len - 1; i >= (int)divisor_len - 1; --i) {
            seq_t factor = dividend[i];
            quotient[i - ((int)divisor_len - 1)] = factor;
            if (factor != 0) {
                for (size_t k = 0; k < divisor_len; ++k)
                    dividend[i - divisor_len + 1 + k] -= factor * divisor[k];
            }
        }

        initialized = 1;
    }

    /* coefficients exist only up to degree 240; otherwise 0 */
    return (n < 241) ? quotient[(size_t)n] : (seq_t)0;
}
