/*
 * A074558  a(n) = 3^n + 7^n + 8^n.
 * https://oeis.org/A074558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   _pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_local.h"

seq_t a(idx_t n)
{
    seq_t p3 = _pow_seq_local(3, n);
    seq_t p7 = _pow_seq_local(7, n);
    seq_t p8 = _pow_seq_local(8, n);
    return p3 + p7 + p8;
}
