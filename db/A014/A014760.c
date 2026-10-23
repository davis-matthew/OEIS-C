/*
 * A014760  Squares of numbers in array formed from odd elements to the right
 *          of middle of rows of Pascal triangle that are not 1.
 * https://oeis.org/A014760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (square of the n-th odd binomial coefficient to the right
 *            of the middle of Pascal's triangle, excluding the coefficient 1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, count_odd_right, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/count_odd_right.h"

seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th term. */
    idx_t r = 0;
    idx_t cum = 0;                                 /* cumulative count so far */
    while (cum <= n) {
        idx_t cnt = count_odd_right(r);
        cum += cnt;
        if (cum > n) break;
        ++r;
    }

    /* Offset of the desired term within its row. */
    idx_t before = cum - count_odd_right(r);       /* terms before row r */
    idx_t offset = n - before;                     /* 0‑based within row r */

    /* Scan the right‑hand side of row r to find the (offset)-th odd entry. */
    idx_t start = (r / 2) + 1;                     /* first index > middle */
    idx_t end   = r - 1;                           /* exclude the final 1 at i = r */
    idx_t i = start;
    idx_t seen = 0;

    for (;; ++i) {
        if (((i & (r - i)) == 0)) {               /* odd binomial coefficient */
            if (seen == offset) break;
            ++seen;
        }
        if (i > end) break;                       /* safety guard */
    }

    /* Compute the binomial coefficient and return its square. */
    seq_t val = binomial(r, i);
    return seq_mul(val, val);
}
