/*
 * A062715  Triangle T(i,j) (i >= -1, -1<=j<=i) whose (i,j)-th entry is 1 if
 *          j=-1 otherwise binomial(i,j)*2^(i-j).
 * https://oeis.org/A062715
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 92, u32 n <= 281, u64 n <= 1000,
 *            u128 n <= 3595, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A062715
   Triangle T(i,j) (i ≥ -1, -1 ≤ j ≤ i):
       T(i,j) = 1    if j = -1
                binomial(i,j) * 2^(i-j)  otherwise
   Flattened row-wise: first T(-1,-1), then row i=0,1,2,...
   For a given n ≥ -1, find the row i and position within that row
   and return the corresponding value.
*/

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Return the n‑th term of A062715 for n ≥ -1
   (undefined for n < -1). */
seq_t a(idx_t n)
{
    if (n < -1) {
        /* behavior below the offset is unspecified */
        return (seq_t)0;
    }

    /* Global zero‑based index (a(-1) → 0). */
    idx_t pos = n + 1;

    /* Find the smallest i such that cumulative(i) > pos,
       where cumulative(i) = (i+2)*(i+3)/2. */
    idx_t i = -1;
    while ((((i + 2) * (i + 3)) / 2) <= pos) {
        ++i;
    }

    /* Number of elements before row i. */
    idx_t prev_total = ((i + 1) * (i + 2)) / 2;

    /* Zero‑based offset within row i. */
    idx_t offset = pos - prev_total;

    /* If offset == 0, column j = -1 → value 1. */
    if (offset == 0) {
        return (seq_t)1;
    }

    /* j ranges from 0 to i. */
    idx_t j = offset - 1;       /* j = offset-1  (because offset=1 → j=0) */
    idx_t exp = i - j;          /* exponent for 2^ */

    return binomial(i, j) * pow2(exp);
}
