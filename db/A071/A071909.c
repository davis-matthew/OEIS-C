/*
 * A071909  A002487(n)*A002487(n+1)*A002487(n+2)/2.
 * https://oeis.org/A071909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stern(n) * Stern(n+1) * Stern(n+2) / 2
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _stern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_stern.h"
/* Stern's diatomic (OEIS A002487): s(0)=0, s(1)=1, s(2n)=s(n), s(2n+1)=s(n)+s(n+1) */

seq_t a(idx_t n) {
    seq_t s0 = _stern(n);
    seq_t s1 = _stern(n + 1);
    seq_t s2 = _stern(n + 2);

    seq_t prod = s0 * s1;
    prod = prod * s2;
    return prod / 2;
}
