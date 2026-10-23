/*
 * A037557  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer whose base‑8 digits are the first n terms of the
 *            periodic sequence 2,1,1,2,1,1,… (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* digits: 2,1,1 repeating; index i is 0‑based */
        seq_t digit = (i % 3 == 0) ? 2 : 1;
        val = val * (seq_t)8 + digit;
    }
    return val;
}
