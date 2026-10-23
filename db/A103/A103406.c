/*
 * A103406  Triangle read by rows: n-th row = unsigned coefficients of the
 *          characteristic polynomials of an n X n matrix with 2's on the
 *          diagonal and 1's elsewhere.
 * https://oeis.org/A103406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (c+1) * binom(r, c) where (r, c) are the row and column
 *            obtained by writing the index n in row‑major order of the
 *            triangle T(r,c) = (c+1)*binom(r,c)
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 510, u64 n <= 2044,
 *            u128 n <= 5150, big any n
 * Helpers:   binom_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_calc.h"

seq_t a(idx_t n)
{
    /* Find row r such that cumulative elements up to previous row <= n */
    idx_t r   = 0;
    idx_t cum = 0;   /* number of elements before row r */

    while (1) {
        idx_t next = cum + r + 1;          /* total elements up to row r */
        if (next > n) break;
        cum = next;
        ++r;
    }

    /* Column within the row */
    idx_t c = n - cum;                     /* 0 ≤ c ≤ r */

    /* Binomial coefficient binom(r, c) */
    seq_t bin = binom_calc(r, c);

    /* Result: (c+1) * binom(r, c) */
    seq_t result = ((seq_t)(c + 1u)) * bin;
    return result;
}
