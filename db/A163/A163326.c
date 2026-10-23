/*
 * A163326  Pick digits at the odd distance from the least significant end of
 *          the ternary expansion of n, then convert back to decimal.
 * https://oeis.org/A163326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} ((n / 3^{2i+1}) mod 3) * 3^i
 * Range:     u8 n <= 728, u16 n <= 728, u32 n <= 728, u64 n <= 728,
 *            u128 n <= 728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, write n in base‑3, pick the digits at odd positions
   (counting from 0 at the least significant end), and
   interpret the picked digits as a base‑3 number.  */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;       /* accumulated result */
    seq_t mult = 1;      /* current power of 3 for the selected digits */
    idx_t pos = 0;       /* position of current ternary digit (0 = LSB) */

    while (n) {
        uint8_t digit = (uint8_t)(n % 3);   /* current digit of n in base 3 */
        if (pos & 1U) {                     /* odd position? */
            res += (seq_t)digit * mult;     /* add in this digit */
            mult *= (seq_t)3;               /* next power of 3 for following digit */
        }
        n /= (idx_t)3;
        pos++;
    }
    return res;
}
