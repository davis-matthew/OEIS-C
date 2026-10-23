/*
 * A037663  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037663
   Formula: a(n) = number whose base‑7 representation consists of the
   first n digits of the periodic sequence 3,1,0.
   That is, a(1)=3, a(2)=3·7+1=22, a(3)=3·7²+1·7+0=154, and so on.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           // offset is 1; 0 is never requested
    seq_t val = 0;
    idx_t i;
    for (i = 0; i < n; ++i) {
        /* digits repeat 3,1,0; i modulo 3 picks the digit in order */
        seq_t d = (i % 3 == 0) ? (seq_t)3
                               : (i % 3 == 1) ? (seq_t)1
                                              : (seq_t)0;
        val = val * (seq_t)7 + d;
    }
    return val;
}
