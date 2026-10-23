/*
 * A037533  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the base‑8 number formed by the first n digits of the
 *            periodic sequence 1,1,2,1,1,2,
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes a(n) by accumulating digits:  a_{k+1} = a_k * 8 + d_k,
   where d_k = 1 for k % 3 != 2 and d_k = 2 otherwise. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        int d = (i % 3 == 2) ? 2 : 1;      /* digit for position i */
        val = val * 8 + (seq_t)d;
    }
    return val;
}
