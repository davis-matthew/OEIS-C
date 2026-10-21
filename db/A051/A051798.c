/*
 * A051798  a(n) = (n+1)*(n+2)*(n+3)*(9n+4)/24.
 * https://oeis.org/A051798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(9n+4)/24
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 324, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    seq_t t = (k + 1) * (k + 2) * (k + 3) * ((seq_t)9 * k + (seq_t)4);
    return t / (seq_t)24;
}
