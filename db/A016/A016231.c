/*
 * A016231  Inverse of 2222nd cyclotomic polynomial.
 * https://oeis.org/A016231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Inverse of the 2222nd cyclotomic polynomial. * a(n) is periodic
 *            with period 2222. * a(n) = 1 if n mod 2222 ∈ {0,2,4,6,8,10} *
 *            a(n) = -1 if n mod 2222 ∈ {1,3,5,7,9} * a(n) = 0 otherwise
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of the period */
    static seq_t period[2222];
    static bool init = false;
    if (!init) {
        for (idx_t i = 0; i < 2222; ++i) {
            if (i <= 10) {
                period[i] = (i % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            } else {
                period[i] = (seq_t)0;
            }
        }
        init = true;
    }
    idx_t r = n % (idx_t)2222;
    return period[r];
}
