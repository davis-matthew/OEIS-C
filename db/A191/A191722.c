/*
 * A191722  Dispersion of A008851, (numbers >1 and congruent to 0 or 1 mod 5),
 *          by antidiagonals.
 * https://oeis.org/A191722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of an infinite table T read along
 *            antidiagonals. The table is defined by T[i][1] = (i==1 ? 1 :
 *            5*((i-2)/3) + (i-2)%3 + 2); T[i][j+1] = 5*((T[i][j]+1)/2) +
 *            ((T[i][j]&1)==0); The pair (i,j) for the nth element satisfies
 *            count_{k=1}^{d-1}k < n ≤ count_{k=1}^d k (d = diagonal...
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 300, u64 n <= 1176,
 *            u128 n <= 4656, big any n
 * Helpers:   nextS
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nextS.h"

static inline seq_t row_start(idx_t i)
{
    if (i == 1) return 1;
    idx_t k = i - 2;                /* zero‑based index beyond the first start */
    idx_t block  = k / 3;           /* groups of three numbers per 5‑block    */
    idx_t offset = k % 3;           /* position within the block             */
    return (seq_t)(5 * block + offset + 2);
}

seq_t a(idx_t n)
{
    /* Find diagonal d and cumulative count of elements before it */
    idx_t d = 1;                     /* current diagonal number                 */
    idx_t count = 0;                /* count of elements up to diagonal d-1    */
    while (count + d < n) {          /* add the current diagonal size if needed */
        count += d;
        ++d;
    }

    /* Position within diagonal d */
    idx_t t = n - count;             /* 1‑based index inside the diagonal     */
    idx_t i = t;                     /* row index (1‑based)                    */
    idx_t j = d - t + 1;             /* column index (1‑based)                */

    /* Compute the table entry T[i][j] */
    seq_t val = row_start(i);
    for (idx_t step = 1; step < j; ++step)
        val = nextS(val);

    return val;
}
