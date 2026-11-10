/*
 * A089601  Interleaving of A089591 and A089600.
 * https://oeis.org/A089601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A089591(n)/10), with A089591(0)=0,
 *            A089591(2k+1)=10*A089591(k)+1, A089591(2k+2)=10*(A089591(k)+1)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A089601_lazy(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n & 1) {
        seq_t sub = A089601_lazy((n - 1) / 2);
        return sub * 10 + 1;
    } else {
        seq_t sub = A089601_lazy((n - 2) / 2);
        seq_t b = (sub / 10) * 10 + (sub % 10 + 1);
        return b * 10;
    }
}

seq_t a(idx_t n)
{
    return A089601_lazy(n) / 10;
}
