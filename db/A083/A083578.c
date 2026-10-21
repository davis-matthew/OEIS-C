/*
 * A083578  a(n) = (6^n + (-4)^n)/2.
 * https://oeis.org/A083578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n + (-4)^n)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes 6^n and 4^n using exponentiation by squaring.
   Then applies the formula depending on the parity of n. */
seq_t a(idx_t n)
{
    seq_t pow6 = 1;
    seq_t pow4 = 1;
    seq_t base6 = 6;
    seq_t base4 = 4;
    idx_t e = n;

    while (e > 0) {
        if (e & 1) {
            pow6 *= base6;
            pow4 *= base4;
        }
        base6 *= base6;
        base4 *= base4;
        e >>= 1;
    }

    if ((n & 1) == 0) {               /* n even: (6^n + 4^n)/2 */
        return (pow6 + pow4) / 2;
    } else {                           /* n odd: (6^n - 4^n)/2 */
        return (pow6 - pow4) / 2;
    }
}
