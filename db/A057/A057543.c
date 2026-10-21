/*
 * A057543  Maximum cycle length (orbit size) in the rotation permutation of
 *          2n non-crossing handshakes.
 * https://oeis.org/A057543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=0, 1 for n=1, 2 for n=2, 3 for n=3, a(n) = 2*n
 *            for all n >= 4
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* For n >= 4, the maximum cycle length is simply 2*n */
    return (seq_t)n * (seq_t)2;
}
