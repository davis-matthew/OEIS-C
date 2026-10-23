/*
 * A173598  Period 6: repeat [1, 8, 7, 2, 4, 5].
 * https://oeis.org/A173598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6), with a(0)=1, a(1)=8, a(2)=7, a(3)=2, a(4)=4,
 *            a(5)=5
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[6] = {1, 8, 7, 2, 4, 5};
    return pat[(size_t)(n % 6)];
}
