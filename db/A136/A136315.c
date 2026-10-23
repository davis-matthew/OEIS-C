/*
 * A136315  Period 10: repeat 1, 2, 3, 6, 5, 0, 7, 4, 9, 8 .
 * https://oeis.org/A136315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1,2,3,6,5,0,7,4,9,8}[n mod 10]
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[10] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6,
        (seq_t)5, (seq_t)0, (seq_t)7, (seq_t)4,
        (seq_t)9, (seq_t)8
    };
    return pattern[(size_t)(n % 10)];
}
