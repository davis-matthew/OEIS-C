/*
 * A014812  Inverse of 803rd cyclotomic polynomial.
 * https://oeis.org/A014812
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

/*  Computation of OEIS A014812:
    a(n) = coefficient of x^n in the formal power series expansion of 1/Φ₈₀₃(x),
    where Φ₈₀₃(x) is the 803rd cyclotomic polynomial.  The sequence is periodic
    with period 803.                                                     */
#include <stddef.h>

#define PERIOD 803

seq_t a(idx_t n)
{
    static seq_t invDen[721];
    static seq_t phi[721];
    static seq_t seq[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        /*  Compute the reciprocal of Den(x) = (x^73-1)(x^11-1)
            up to degree 720.  Den(0)=1, so the inverse exists.        */
        invDen[0] = (seq_t)1;
        for (size_t i = 1; i <= 720; ++i) {
            seq_t sum = 0;
            if (i >= 11) sum -= invDen[i - 11];   /* D[11] = -1          */
            if (i >= 73) sum -= invDen[i - 73];   /* D[73] = -1          */
            if (i >= 84) sum += invDen[i - 84];   /* D[84] = +1          */
            invDen[i] = -sum;
        }

        /*  φ_k = coefficient of x^k in Φ₈₀₃(x) for k = 0..720     */
        phi[0] = invDen[0];
        for (size_t k = 1; k <= 720; ++k) {
            phi[k] = invDen[k] - invDen[k - 1];
        }

        /*  Compute first 803 terms of the inverse series:   */
        seq[0] = (seq_t)1; /* because φ₀ = 1 */
        for (size_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            size_t maxk = i < 720 ? i : 720;
            for (size_t k = 1; k <= maxk; ++k) {
                sum += phi[k] * seq[i - k];
            }
            seq[i] = -sum;
        }

        initialized = 1;
    }

    return seq[(size_t)(n % PERIOD)];
}
