/*
 * A040509  Continued fraction for sqrt(533).
 * https://oeis.org/A040509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction coefficients of sqrt(533).
   a(0) = 23.
   For n ≥ 1 the coefficients repeat the period [11, 1, 1, 11, 46].  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    static const seq_t period[5] = { (seq_t)11, (seq_t)1, (seq_t)1,
                                     (seq_t)11, (seq_t)46 };

    /* (n-1) is safe because n>0 here */
    return period[(n - 1) % 5];
}
