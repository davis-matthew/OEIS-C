/*
 * A219009  Coefficients of the Dirichlet series for zeta(4s)/zeta(s).
 * https://oeis.org/A219009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e || n of (-1)^e if e % 4 ==
 *            0 or 1; otherwise 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        uint64_t mod4 = e & 3;
        if (mod4 == 2 || mod4 == 3) return 0;
        if (e & 1) result = -result;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t mod4 = e & 3;
            if (mod4 == 2 || mod4 == 3) return 0;
            if (e & 1) result = -result;
        }
    }

    /* Remaining prime factor (if any) with exponent 1 */
    if (m > 1) {
        result = -result;
    }

    return result;
}
