/*
 * A021027  Decimal expansion of 1/23.
 * https://oeis.org/A021027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th digit in the decimal expansion of 1/23, with
 *            a(0) = 0 (the integer part). For n ≥ 1 it repeats the 22‑digit
 *            cycle 4,3,4,7,8,2,6,0,8,6,9,5,6,5,2,1,7,3,9,1,3,0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[22] = {4, 3, 4, 7, 8, 2, 6, 0, 8, 6, 9, 5,
                                  6, 5, 2, 1, 7, 3, 9, 1, 3, 0};
    if (n == 0)
        return (seq_t)0;
    size_t idx = (size_t)((n - 1) % 22);
    return pat[idx];
}
