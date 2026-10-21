/*
 * A097399  Maximum of the determinant over all permutations of the entries of
 *          a 3 X 3 matrix which are consecutive integers in the range
 *          (n-4,n+4).
 * https://oeis.org/A097399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 86, a(1) = 104, a(n) = 80*n + 12 for 2 <= n <= 12, a(n) =
 *            81*n for n >= 13
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) { return 86; }
    if (n == 1u) { return 104; }
    if (n < 13u) { return (seq_t)80 * n + 12; }
    return (seq_t)81 * n;
}
