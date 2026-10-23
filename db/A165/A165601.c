/*
 * A165601  Midpoint height of Jacobi-bridge, computed for 4n+3. a(n) =
 *          Sum_{i=0..(2n+1)} J(i,4n+3), where J(i,m) is the Jacobi symbol.
 * https://oeis.org/A165601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0}^{2 n + 1} J(i, 4 n + 3) where J(i,m) is the
 *            Jacobi symbol
 * Range:     u8 n <= 5641, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The jacobi_symbol function prototype is provided in the utils library. */
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Compute m = 4 n + 3 (always odd). */
    uint64_t m = 4 * n + 3;

    /* Summation range: i = 0 .. 2 n + 1. */
    idx_t max_i = 2 * n + 1;

    /* Use a signed accumulator; the absolute value will never overflow. */
    int64_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i)
    {
        int j = jacobi_symbol(i, m);
        sum += (int64_t)j;
    }

    /* Convert the signed result to an unsigned seq_t. */
    return (seq_t)sum;
}
