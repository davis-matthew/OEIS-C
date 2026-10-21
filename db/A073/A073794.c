/*
 * A073794  Replace 7^k with (-7)^k in base 7 expansion of n.
 * https://oeis.org/A073794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(7*k+m) = -7*a(k) + m with 0 <= m < 7, a(0)=0,
 *            a(1)=1,...,a(6)=6. Equivalent: a(n) = sum of base‑7 digits of n
 *            multiplied by (-7)^position
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power   = 1;          /* (-7)^0 */
    idx_t temp = n;

    while (temp) {
        seq_t digit = temp % 7; /* base‑7 digit */
        result += digit * power;
        temp   /= 7;
        power  *= (seq_t)(-7);
    }

    return result;
}
