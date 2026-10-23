/*
 * A014720  Squares of elements to right of central element in Pascal triangle
 *          (by row) that are not 1.
 * https://oeis.org/A014720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: squares of non‑1 elements to the right of the central
   element in each row of Pascal's triangle.
   Formula: a(n) = A014411(n)^2   (A014411 collects the non‑1 RHS entries) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first RHS entry (row 3, entry 2). */
    if (n == 0)
        return 0;

    idx_t remaining = n;
    for (idx_t r = 3; ; ++r)        /* r is the row number (0‑based) */
    {
        /* number of RHS entries in row r (excluding the final 1) */
        idx_t count = r - 1 - (r >> 1);

        if (remaining <= count)
        {
            /* position within this row (0‑based) */
            idx_t offset = remaining - 1;
            /* first RHS index is floor(r/2)+1 */
            idx_t start = (r >> 1) + 1;
            idx_t k     = start + offset;   /* actual binomial index */
            seq_t val   = binomial(r, k);   /* value of C(r,k) */
            return val * val;               /* square it */
        }
        remaining -= count;
    }
}
