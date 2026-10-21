/*
 * A240910  The sequency numbers of the 32 rows of a Hadamard-Walsh matrix,
 *          order 32.
 * https://oeis.org/A240910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of sign changes in the (n‑1)-th row of the *
 *            Hadamard matrix of order 32 (H(5)). The rows are indexed from 0.
 *            * The row values are given by H[i][j] = (-1)^{popcount(i & j)}.
 *            The * number of sign changes is counted between consecutive
 *            columns j * and j+1 (j = 0 … 30). This implementa...
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   parity32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity32.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n = 1 … 32; for n > 32
       we return 0 as a harmless default. */
    if (n == 0) return (seq_t)0;        /* undefined, but safe */
    idx_t idx = n - 1;
    if (idx >= 32) return (seq_t)0;     /* out of defined range */

    uint32_t row = (uint32_t)idx;
    uint32_t count = 0;

    /* count transitions between j and j+1 for j = 0 … 30 */
    for (uint32_t j = 0; j < 31; ++j) {
        uint32_t p1 = parity32(row & j);
        uint32_t p2 = parity32(row & (j + 1));
        count += p1 ^ p2;               /* add 1 if signs differ */
    }
    return (seq_t)count;
}
