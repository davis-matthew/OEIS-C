/*
 * A095427  a(n) = 12 written in base n.
 * https://oeis.org/A095427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 expressed in base n, then interpreted as a decimal
 *            number * (i.e. the digits of the base‑n representation are read
 *            in base 10). * Base 1 is treated specially: it yields a unary
 *            representation of length 12, * i.e. 111111111111
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* Special case: base 1 (unary) */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 12; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }
 
    /* Compute digits of 12 in base n (least significant first) */
    idx_t value = 12;                     /* 12 in decimal */
    uint8_t digits[64];                    /* enough for any n >= 2 */
    int dcnt = 0;
    while (value > 0) {
        digits[dcnt++] = value % n;
        value /= n;
    }
    /* Build the decimal number from most significant digit to least */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
