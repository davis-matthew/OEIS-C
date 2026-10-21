/*
 * A225240  The squares on a chessboard that are white, counting from top left
 *          corner and down.
 * https://oeis.org/A225240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   let k = floor((n-1)/4), t = (n-1) mod 4 a(n) = 8*k + 2*t + ( (k
 *            is even) ? 1 : 2 ) (This enumerates the indices of squares with
 *            even row+col parity on an infinite chessboard, row-major from
 *            the top-left corner.)
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence offset is 1; guard for safety */

    /* Compute k and t using the integer division of the index type. */
    idx_t n_minus_1 = n - 1;
    idx_t k_idx = n_minus_1 / 4;
    idx_t t_idx = n_minus_1 % 4;

    /* Convert to seq_t for arithmetic on the result type. */
    seq_t k = (seq_t)k_idx;
    seq_t t = (seq_t)t_idx;

    /* Determine if k is even or odd. */
    int k_even = ((k_idx & 1) == 0);

    /* Final formula in seq_t arithmetic. */
    seq_t result = 8 * k + 2 * t + (k_even ? 1 : 2);
    return result;
}
