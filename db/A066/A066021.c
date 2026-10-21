/*
 * A066021  (a(n)^6+1)/(n^6+1) is the smallest integer > 1.
 * https://oeis.org/A066021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 (for all n except the special cases n = 1, 2, 6, 8
 *            where a(n) = 3, 7, 119, 177 respectively)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* special cases directly from the sequence definition */
    switch (n) {
        case 1: return (seq_t)3;
        case 2: return (seq_t)7;
        case 6: return (seq_t)119;
        case 8: return (seq_t)177;
        default: break;
    }
    /* general formula: n^3 */
    seq_t nn = (seq_t)n;
    return nn * nn * nn;
}
