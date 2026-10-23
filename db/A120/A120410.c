/*
 * A120410  a(n) =
 *          n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5*(n+5)^6*(n+6)^7/(1!*2!*3!*4!*5!*6!*7!).
 * https://oeis.org/A120410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5*(n+5)^6*(n+6)^7 *
 *            /(1!*2!*3!*4!*5!*6!*7!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    seq_t res = 1;

    /* Prime exponents of the denominator 1!*2!*…*7! */
    unsigned rem2 = 16;   /* 2^16  */
    unsigned rem3 = 7;    /* 3^7   */
    unsigned rem5 = 3;    /* 5^3   */
    unsigned rem7 = 1;    /* 7^1   */

    for (idx_t k = 0; k <= 6; ++k) {
        seq_t base = (seq_t)n + k;
        for (idx_t e = 1; e <= k + 1; ++e) {
            res *= base;

            /* cancel factors of the denominator whenever possible */
            while (rem2 > 0 && (res % 2 == 0)) {
                res /= 2;
                --rem2;
            }
            while (rem3 > 0 && (res % 3 == 0)) {
                res /= 3;
                --rem3;
            }
            while (rem5 > 0 && (res % 5 == 0)) {
                res /= 5;
                --rem5;
            }
            while (rem7 > 0 && (res % 7 == 0)) {
                res /= 7;
                --rem7;
            }
        }
    }

    /* Final cleanup: divide by any remaining factors (should be none) */
    while (rem2 > 0) {
        if (res % 2 == 0) { res /= 2; --rem2; }
        else break;
    }
    while (rem3 > 0) {
        if (res % 3 == 0) { res /= 3; --rem3; }
        else break;
    }
    while (rem5 > 0) {
        if (res % 5 == 0) { res /= 5; --rem5; }
        else break;
    }
    while (rem7 > 0) {
        if (res % 7 == 0) { res /= 7; --rem7; }
        else break;
    }

    return res;
}
