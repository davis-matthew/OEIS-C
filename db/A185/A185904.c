/*
 * A185904  Multiplication table for the tetrahedral numbers (A000292), by
 *          antidiagonals.
 * https://oeis.org/A185904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(i+2,3) * binom(j+2,3) where (i,j) is the
 *            antidiagonal coordinate pair for index n (computed below)
 * Range:     u8 n <= 23, u16 n <= 217, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom, find_S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/find_S.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* sequence is defined only for n>=1 */

    /* The diagonal on which n lies */
    idx_t S = find_S(n);

    /* How many terms precede this diagonal */
    __int128 prev_val = (__int128)(S - 1) * (S - 2) / 2;
    idx_t prev = (idx_t)prev_val;

    /* Position of n on the diagonal: 1..S-1 */
    idx_t offset = n - prev;

    /* Antidiagonal coordinates (i,j) with i+j = S and i>=1 */
    idx_t i = offset;
    idx_t j = S - i;

    /* Tetrahedral numbers for i and j */
    seq_t ti = binom((idx_t)(i + 2), (idx_t)3);
    seq_t tj = binom((idx_t)(j + 2), (idx_t)3);

    return ti * tj;
}
