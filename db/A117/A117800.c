/*
 * A117800  Start with 1 and repeatedly reverse the digits and add 5 to get
 *          the next term.
 * https://oeis.org/A117800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(n) = reverse_decimal(a(n-1)) + 5 for n≥1
 * Range:     u8 n <= 17, u16 n <= 414, u32 n <= 414, u64 n <= 414,
 *            u128 n <= 414, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t cur = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        cur = reverse_decimal(cur) + (seq_t)5;
    }
    return cur;
}
