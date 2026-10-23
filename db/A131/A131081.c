/*
 * A131081  Periodic sequence (8, 7, 4, 1, 0, 1, 4, 7).
 * https://oeis.org/A131081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8,7,4,1,0,1,4,7 repeated every 8 terms (a(1)=8)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[8] = {
        (seq_t)8, (seq_t)7, (seq_t)4,
        (seq_t)1, (seq_t)0, (seq_t)1,
        (seq_t)4, (seq_t)7
    };
    idx_t r = (n - 1) % (idx_t)8;
    return period[(size_t)r];
}
