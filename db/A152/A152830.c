/*
 * A152830  Triangle read by rows, A007318 rows repeated four times .
 * https://oeis.org/A152830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 266, u16 n <= 766, u32 n <= 2257, u64 n <= 5099,
 *            u128 n <= 5099, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows, A007318 rows repeated four times.
   Each row r (with r≥0) consists of binomial coefficients C(r,k)
   for k=0..r, and this whole row is repeated four times.
   The sequence starts with offset 0. */
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t r = 0;          /* current row index */
    idx_t start = 0;      /* linear position where this row begins */

    /* Find the row that contains position n. */
    while (1) {
        idx_t len   = r + 1;          /* length of row r */
        idx_t block = len << 2;       /* 4 × len  (safe for small n) */
        if (n < start + block)
            break;
        start += block;
        ++r;
    }

    /* Position of n inside the 4 repeated copies of the row. */
    idx_t len   = r + 1;
    idx_t pos   = n - start;        /* 0 <= pos < 4*len */
    idx_t k     = pos % len;        /* column index within the row */

    return binom(r, k);
}
