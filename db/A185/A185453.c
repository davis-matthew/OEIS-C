/*
 * A185453  Trajectory of 1 under repeated application of the map in A185452.
 * https://oeis.org/A185453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1,3,8,4,2} repeated with period 5, i.e.,
 *            a(n)=cycle[(n-1)%5]
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t cycle[5] = {(seq_t)1, (seq_t)3, (seq_t)8, (seq_t)4, (seq_t)2};
    idx_t m = (n - 1) % 5;
    return cycle[(size_t)m];
}
