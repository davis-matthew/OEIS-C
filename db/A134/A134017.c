/*
 * A134017  Period 9: repeat [1, 2, 4, 3, 5, 3, 4, 2, 1].
 * https://oeis.org/A134017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) with period 9: [1,2,4,3,5,3,4,2,1] offset 0
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[9] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)3, (seq_t)5,
        (seq_t)3, (seq_t)4, (seq_t)2, (seq_t)1
    };
    return pat[(size_t)(n % (idx_t)9)];
}
