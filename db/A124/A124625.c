/*
 * A124625  Even numbers sandwiched between 1's.
 * https://oeis.org/A124625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even, a(n) = n-1 if n is odd
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Even indices yield 1; odd indices yield n-1 */
    return ((n & (idx_t)1) == 0) ? (seq_t)1 : ((seq_t)n - (seq_t)1);
}
