/*
 * A108020  a(n) is the number whose binary representation is the
 *          concatenation of n strings of the four digits "1100".
 * https://oeis.org/A108020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n‑1) + 12, with a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)16 + (seq_t)12;
    }
    return res;
}
