/*
 * A089591  "Lazy binary" representation of n. Also called redundant binary
 *          representation of n.
 * https://oeis.org/A089591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, base
 * Formula:   a(0) = 0. For n > 0: if n is odd, a(n) = a((n-1)/2) * 10 + 1 if
 *            n is even, a(n) = ((a((n-2)/2) / 10) * 10 + (a((n-2)/2) % 10 +
 *            1)) * 10 This implements the lazy‑binary recurrence described in
 *            the OEIS entry
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1534, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n & 1ull) /* odd */
    {
        seq_t sub = a((n - 1) / 2);
        return sub * 10 + 1;
    }
    else         /* even */
    {
        seq_t sub = a((n - 2) / 2);
        seq_t b   = (sub / 10) * 10 + (sub % 10 + 1);
        return b * 10;
    }
}
