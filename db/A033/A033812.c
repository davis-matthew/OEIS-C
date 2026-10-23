/*
 * A033812  The Loh-Shu 3 X 3 magic square, lexicographically largest variant
 *          when read by columns.
 * https://oeis.org/A033812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full, tabf
 * Formula:   a(n) = pattern[(n-1) mod 9], pattern = {8,1,6,3,5,7,4,9,2}
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[9] = {8,1,6,3,5,7,4,9,2};
    return pattern[(size_t)((n - 1) % 9)];
}
