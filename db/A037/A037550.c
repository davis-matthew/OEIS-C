/*
 * A037550  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose base‑9 representation is the first n terms
 *            of the periodic sequence 1,2,2,1,2,2,... (i.e. digits d_k =
 *            {1,2,2}[ (k-1) mod 3 ]) Compute by iteratively building the
 *            decimal value: a(0) = 0; a(n) = a(n-1)*9 + d_n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    const int pat[3] = {1, 2, 2};

    for (idx_t i = 0; i < n; ++i) {
        val = val * 9 + (seq_t)pat[i % 3];
    }
    return val;
}
