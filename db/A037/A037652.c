/*
 * A037652  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,0,1.
 * https://oeis.org/A037652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((301/999)*10^n) Sequence is formed by the periodic
 *            decimal pattern 3,0,1.\n a(1)=3, a(2)=30, a(3)=301, … This
 *            implementation constructs the n‑digit number by appending the
 *            corresponding digit from the period. No additional memory is
 *            needed
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char pat[3] = {3, 0, 1};
    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned digit = pat[(i - 1) % 3];
        result = result * (seq_t)10 + (seq_t)digit;
    }
    return result;
}
