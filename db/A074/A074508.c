/*
 * A074508  a(n) = 1^n + 3^n + 6^n.
 * https://oeis.org/A074508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 3^n + 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow3 = (seq_t)1;
    seq_t pow6 = (seq_t)1;
    seq_t base3 = (seq_t)3;
    seq_t base6 = (seq_t)6;
    idx_t exp = n;

    /* compute 3^n */
    while (exp > 0) {
        if (exp & 1U) {
            pow3 *= base3;
        }
        base3 *= base3;
        exp >>= 1;
    }

    exp = n;
    /* compute 6^n */
    while (exp > 0) {
        if (exp & 1U) {
            pow6 *= base6;
        }
        base6 *= base6;
        exp >>= 1;
    }

    return (seq_t)1 + pow3 + pow6;
}
