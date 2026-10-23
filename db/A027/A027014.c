/*
 * A027014  a(n) = T(2*n, n+3), T given by A027011.
 * https://oeis.org/A027014
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(2*n, n+3) where T is the triangle A027011. The
 *            generating function of a(n) has denominator (1-x)^7(1-3x+x^2),
 *            giving the linear recurrence (valid for n ≥ 13): a_n = 10
 *            a_{n-1} - 43 a_{n-2} + 105 a_{n-3} - 161 a_{n-4} + 161 a_{n-5} -
 *            105 a_{n-6} + 43 a_{n-7} - 10 a_{n-8} + a_{n-9} Initial...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour is irrelevant there. */
    if (n < 3) return 0;

    /* Initial values up to a(12). */
    static const seq_t init[] = {
        0, 0, 0,
        1, 10, 203, 1719, 9484, 40615,
        147760, 481849, 1458335, 4194686
    };
    if (n <= 12) return init[n];

    /* Coefficients for the recurrence. */
    static const int coeffs[9] = {
        10, -43, 105, -161, 161, -105, 43, -10, 1
    };

    /* Sliding buffer holding the last 9 values: a_{k-9} … a_{k-1}. */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)           /* buf[0] = a_4, buf[8] = a_12 */
        buf[i] = init[i + 4];

    seq_t value = 0;
    for (idx_t m = 13; m <= n; ++m) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;
        for (int k = 0; k < 9; ++k) {
            /* buffer[8-k] holds a_{m-(k+1)} */
            seq_t term = buf[8 - k] * (seq_t)(coeffs[k] >= 0 ? coeffs[k] : -coeffs[k]);
            if (coeffs[k] >= 0)
                sumPos += term;
            else
                sumNeg += term;
        }
        value = sumPos - sumNeg;           /* a_m */

        /* Shift buffer and insert the new value. */
        for (int i = 0; i < 8; ++i) buf[i] = buf[i + 1];
        buf[8] = value;

        if (m == n) return value;
    }
    /* Should never reach here. */
    return value;
}
