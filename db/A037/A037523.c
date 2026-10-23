/*
 * A037523  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value of the first n digits of the periodic sequence
 *            2,1,0 interpreted as a base‑6 number
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
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
        seq_t digit;
        switch (i % 3) {
            case 0: digit = 2; break;
            case 1: digit = 1; break;
            default: digit = 0; break;
        }
        res = res * 6 + digit;
    }
    return res;
}
