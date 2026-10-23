/*
 * A046231  Numbers whose cube is palindromic in base 4.
 * https://oeis.org/A046231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(1) = 0 a(2) = 1 for n >= 3: a(n) = 4^(n-2) + 1 (the sequence
 *            grows as 4^(n-2)+1 for n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    return pow4(n - 2) + (seq_t)1;
}
