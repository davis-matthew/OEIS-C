/*
 * A037538  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence A037538: a(n) is the integer represented by the first n digits
   of the repeating base‑5 digit sequence 1,2,1 (i.e. 1,2,1,1,2,1,…) */
seq_t a(idx_t n)
{
    static const seq_t pattern[3] = {1, 2, 1};
    seq_t res = 0;

    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)5 + pattern[i % 3];
    }

    return res;
}
