/*
 * A103145  a(n) = (1/6)*(n^3 + 21*n^2 + 74*n + 18).
 * https://oeis.org/A103145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(n^3 + 21*n^2 + 74*n + 18)
 * Range:     u8 n <= 6, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 num = tn * tn * tn
                        + (unsigned __int128)21 * tn * tn
                        + (unsigned __int128)74 * tn
                        + (unsigned __int128)18;
    return (seq_t)(num / 6);
}
