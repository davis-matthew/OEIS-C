/*
 * A070709  n^7 mod 28.
 * https://oeis.org/A070709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 28
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n % 28ULL;   /* reduce n modulo 28 */
    seq_t res = 1;
    for (uint64_t i = 0; i < 7; ++i) {
        /* compute res = (res * m) mod 28 using int arithmetic to avoid
           overflow for small seq_t types (e.g., uint8_t). */
        int tmp = ((int)res * (int)m) % 28;
        res = (seq_t)tmp;
    }
    return res;
}
