/*
 * A014033  Inverse of 24th cyclotomic polynomial.
 * https://oeis.org/A014033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4) - a(n-8); a(0) = 1, a(k)=0 for 1≤k<4.
 *            Equivalently, the sequence is periodic of period 24
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state holds the 24-term period of the sequence */
    static bool initialized = false;
    static seq_t period[24];

    if (!initialized) {
        period[0] = 1;
        for (size_t i = 1; i < 24; ++i) {
            seq_t val = 0;
            if (i >= 4) val += period[i - 4];
            if (i >= 8) val -= period[i - 8];
            period[i] = val;
        }
        initialized = true;
    }
    return period[(size_t)(n % 24)];
}
