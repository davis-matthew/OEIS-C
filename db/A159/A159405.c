/*
 * A159405  Number of n X n arrays of squares of integers summing to 20.
 * https://oeis.org/A159405
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^20 in (1 + x + x^4 + x^9 + x^16)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* total number of variables in the array */
    idx_t m = n * n;          /* n^2 fits in idx_t */

    /* base polynomial g(x) = 1 + x + x^4 + x^9 + x^16 */
    seq_t pow_poly[21] = {0};
    pow_poly[0] = (seq_t)1;
    pow_poly[1] = (seq_t)1;
    pow_poly[4] = (seq_t)1;
    pow_poly[9] = (seq_t)1;
    pow_poly[16] = (seq_t)1;

    /* result polynomial (initially g^0 = 1) */
    seq_t result[21] = {0};
    result[0] = (seq_t)1;

    /* temporary polynomial for multiplications */
    seq_t temp[21];

    /* binary exponentiation: compute pow_poly^m mod x^21 */
    while (m > 0) {
        if (m & (idx_t)1) {
            /* result = result * pow_poly (truncated to degree 20) */
            for (int i = 0; i < 21; i++) temp[i] = (seq_t)0;
            for (int i = 0; i <= 20; i++) {
                if (result[i] == (seq_t)0) continue;
                for (int j = 0; j <= 20 - i; j++) {
                    if (pow_poly[j] == (seq_t)0) continue;
                    temp[i + j] += result[i] * pow_poly[j];
                }
            }
            for (int i = 0; i < 21; i++) result[i] = temp[i];
        }

        /* pow_poly = pow_poly * pow_poly (truncated) */
        for (int i = 0; i < 21; i++) temp[i] = (seq_t)0;
        for (int i = 0; i <= 20; i++) {
            if (pow_poly[i] == (seq_t)0) continue;
            for (int j = 0; j <= 20 - i; j++) {
                if (pow_poly[j] == (seq_t)0) continue;
                temp[i + j] += pow_poly[i] * pow_poly[j];
            }
        }
        for (int i = 0; i < 21; i++) pow_poly[i] = temp[i];

        m >>= 1;
    }

    return result[20];
}
