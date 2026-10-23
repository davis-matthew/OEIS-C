/*
 * A165460  The height at the 1/3 point of Jacobi-bridge, computed for 12n+7.
 *          a(n) = Sum_{i=0..(4n+2)} J(i,12n+7), where J(i,m) is the Jacobi
 *          symbol.
 * https://oeis.org/A165460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..(4n+2)} J(i,12n+7)
 * Range:     u8 n <= 1641, u16 n <= 9818, u32 n <= 9818, u64 n <= 9818,
 *            u128 n <= 9818, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    // m = 12n + 7 (always odd)
    idx_t m   = 12 * n + 7;
    // last term index
    idx_t end = 4 * n + 2;

    int sum = 0;
    for (idx_t i = 0; i <= end; ++i) {
        sum += jacobi_symbol(i, m);
    }
    return (seq_t)sum;
}
