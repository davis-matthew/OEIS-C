/*
 * A037552  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the base‑3 number whose first n digits are the periodic
 *            sequence 2,1,1,2,1,1,.... That is, a(n) = a(n‑1)*3 + d_{n-1}
 *            with d_{0}=2, d_{1}=1, d_{2}=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
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
        if ((i % 3) == 0)
            val = val * 3 + (seq_t)2;
        else
            val = val * 3 + (seq_t)1;
    }
    return val;
}
