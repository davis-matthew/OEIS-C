/*
 * A098608  a(n) = 100^n.
 * https://oeis.org/A098608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes 100^n for any n ≥ 0.  Uses exponentiation by squaring
 * so the number of multiplications is O(log n).  The function works
 * for all supported build types: 8, 16, 32, 64 or 128 bit unsigned
 * integers, as well as the arbitrary‑precision implementation. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)100;

    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n    >>= 1;
    }

    return result;
}
