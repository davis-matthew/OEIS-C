/*
 * A070395  a(n) = 6^n mod 19.
 * https://oeis.org/A070395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 19
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pow6mod19[9] = {
        1, 6, 17, 7, 4, 5, 11, 9, 16
    };
    return pow6mod19[(size_t)(n % 9)];
}
