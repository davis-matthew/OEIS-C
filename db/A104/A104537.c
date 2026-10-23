/*
 * A104537  Expansion of g.f. (1+x)/(1+2*x+4*x^2).
 * https://oeis.org/A104537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n % 3 == 0) ? 2^n : -2^(n-1)
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n % 3 == 0  → a(n) = 2^n
       otherwise    → a(n) = -2^(n-1)  */
    if (n % 3 == 0) {
        return pow2(n);
    } else {
        return -pow2(n - 1);
    }
}
