/*
 * A171654  Period 10: repeat [0, 1, 6, 7, 2, 3, 8, 9, 4, 5].
 * https://oeis.org/A171654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [0,1,6,7,2,3,8,9,4,5](n mod 10)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t PAT[10] = {0, 1, 6, 7, 2, 3, 8, 9, 4, 5};
    return PAT[(size_t)(n % (idx_t)10)];
}
