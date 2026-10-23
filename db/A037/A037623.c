/*
 * A037623  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS entry: A037623
   Base 9 digits are, in order, the first n terms of the periodic sequence with initial period 2,0,3.
   The n‑term yields the integer whose base‑9 representation consists of the first n digits of that periodic
   sequence.  Recurrence:
       a(1) = 2
       a(n) = 9*a(n-1) + d_{n},  where d_{n} = {2,0,3}[ (n-1) mod 3 ]
*/

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is at least 1; treat 1‑based indexing as described */
    static const unsigned int pattern[3] = {2, 0, 3};

    seq_t val = (seq_t)2;           /* a(1) */
    if (n <= 1) return val;

    idx_t i;
    for (i = 2; i <= n; ++i) {
        unsigned int d = pattern[(i - 1) % 3];
        val = val * (seq_t)9 + (seq_t)d;
    }
    return val;
}
