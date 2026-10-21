/*
 * A221684  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*n - 32 for n > 3 Special cases: a(1)=16, a(2)=40,
 *            a(3)=68
 * Range:     u8 n <= 8, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)40;
    if (n == 3) return (seq_t)68;
    /* for n >= 4 */
    return (seq_t)33 * (seq_t)n - (seq_t)32;
}
