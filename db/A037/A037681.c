/*
 * A037681  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the integer represented by the base‑4 number whose rightmost
   digit is the n‑th term of the repeating sequence 1,0,3,2.
   Recurrence: a(0)=0; a(n)=4*a(n-1)+{1,0,3,2}[((n-1) mod 4)] for n≥1. */

#include <stdint.h> /* for idx_t, seq_t definitions */

seq_t a(idx_t n)
{
    static const seq_t period[4] = {1, 0, 3, 2};

    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 4 + period[i & 3];
    return res;
}
