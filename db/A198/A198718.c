/*
 * A198718  Number of nX3 0..6 arrays with values 0..6 introduced in row major
 *          order and no element equal to any horizontal or vertical neighbor.
 * https://oeis.org/A198718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 234*a(n-1) - 13497*a(n-2) + 283640*a(n-3) -
 *            2626919*a(n-4) + 12277210*a(n-5) - 30282843*a(n-6) +
 *            38629620*a(n-7) - 23115780*a(n-8) + 4848336*a(n-9) (valid for n
 *            ≥ 10, with initial values a(1)…a(9) given below)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1) … a(9) */
    static const seq_t init[9] = {
        (seq_t)2,
        (seq_t)34,
        (seq_t)2027,
        (seq_t)232841,
        (seq_t)34003792,
        (seq_t)5315840795,
        (seq_t)846047363854,
        (seq_t)135284283124811,
        (seq_t)21658679381667910
    };

    /* Coefficients of the recurrence (c1 … c9) */
    static const long long coeff[9] = {
        234,
        -13497,
        283640,
        -2626919,
        12277210,
        -30282843,
        38629620,
        -23115780,
        4848336
    };

    if (n <= 9) {
        return init[n - 1];
    }

    /* Buffer holding the last 9 terms: buf[0] = a(k-8), …, buf[8] = a(k) */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms from a(10) up to a(n) */
    for (idx_t k = 10; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 0; j < 9; ++j) {
            /* buf[8-j] corresponds to a(k-j) */
            seq_t term;
            if (coeff[j] >= 0) {
                term = (seq_t)coeff[j] * buf[8 - j];
                sum += term;
            } else {
                term = (seq_t)(-coeff[j]) * buf[8 - j];
                sum -= term;
            }
        }
        /* Shift buffer to drop a(k-8) and append a(k) = sum */
        for (int j = 0; j < 8; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[8] = sum;
    }

    return buf[8];
}
