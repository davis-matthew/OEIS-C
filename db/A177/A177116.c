/*
 * A177116  Partial sums of round(n^2/11).
 * https://oeis.org/A177116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 11*n + 18)/66)
 * Range:     u8 n <= 19, u16 n <= 128, u32 n <= 870, u64 n <= 870,
 *            u128 n <= 870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* use a wider type for temporary calculations */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 val =
          (unsigned __int128)2 * nn * nn * nn
        + (unsigned __int128)3 * nn * nn
        - (unsigned __int128)11 * nn
        + 18;
    return (seq_t)(val / 66);
}
