/*
 * A103469  Number of polyominoes consisting of 3 regular unit n-gons.
 * https://oeis.org/A103469
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-2)/2) - floor((n-1)/6) + 1
 * Range:     u8 n <= 358, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* out of offset range */
    seq_t term1 = (seq_t)(n - 2) / 2;         /* floor((n-2)/2)   */
    seq_t term2 = (seq_t)(n - 1) / 6;         /* floor((n-1)/6)   */
    return term1 - term2 + 1;
}
