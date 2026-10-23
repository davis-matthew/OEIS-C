/*
 * A028735  Nonsquares mod 22.
 * https://oeis.org/A028735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 22*floor((n-1)/10) + t[(n-1) % 10] where * t =
 *            [2,6,7,8,10,13,17,18,19,21] are the non‑residues modulo 22
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _nonres[10] = {2,6,7,8,10,13,17,18,19,21};

seq_t a(idx_t n)
{
    /* Defined for n ≥ 1. For n == 0 return an arbitrary value. */
    if (n == 0)
        return 0;

    idx_t pos   = n - 1;
    idx_t cycle = pos / 10;
    idx_t rem   = pos % 10;

    seq_t val = (seq_t)22 * (seq_t)cycle + _nonres[(size_t)rem];
    return val;
}
