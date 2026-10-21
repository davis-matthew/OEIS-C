/*
 * A217395  Numbers starting with 3.
 * https://oeis.org/A217395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (26*10^floor(log10(9*n-8))-8)/9
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 9*n-8 as index type */
    idx_t m = 9 * n - 8;

    /* find largest power of 10 <= m */
    idx_t pow10 = 1;
    while (pow10 * 10 <= m) {
        pow10 *= 10;
    }
    seq_t pow10_seq = (seq_t)pow10;

    /* apply formula */
    seq_t term = (seq_t)n + ((26 * pow10_seq - 8) / 9);
    return term;
}
