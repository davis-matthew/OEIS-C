/*
 * A216606  Decimal expansion of 360/7.
 * https://oeis.org/A216606
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, less, easy
 * Formula:   a(2) = 5; For n ≥ 3, a(n) = cycle[(n-3) mod 6] with cycle = {1,
 *            4, 2, 8, 5, 7}
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;        /* undefined for n < 2 */
    if (n == 2) return (seq_t)5;       /* integer part */

    static const seq_t cycle[6] = { (seq_t)1, (seq_t)4, (seq_t)2,
                                   (seq_t)8, (seq_t)5, (seq_t)7 };
    idx_t pos = (n - 3) % 6;
    return cycle[(size_t)pos];
}
