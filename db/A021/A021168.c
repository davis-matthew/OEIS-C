/*
 * A021168  Decimal expansion of 1/164.
 * https://oeis.org/A021168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point of 1/164. The
 *            decimal expansion is 0.006097560975609..., repeating the 5‑digit
 *            block 6,0,9,7,5 starting at position 2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 */
    if (n < 2)
        return 0;

    /* cycle of 5 digits: 6,0,9,7,5 */
    static const seq_t cycle[5] = {6, 0, 9, 7, 5};

    /* (n-2) % 5 gives the index into the cycle for n ≥ 2 */
    idx_t rem = (n - 2) % 5;
    return cycle[(size_t)rem];
}
