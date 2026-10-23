/*
 * A037637  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑9 representation is the first n digits
 *            of the periodic sequence 2,1,3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[3] = { (seq_t)2, (seq_t)1, (seq_t)3 };
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t d = pat[(size_t)(i % 3)];
        res = res * (seq_t)9 + d;
    }
    return res;
}
