/*
 * A210257  Construct a triangle with centered rows in which row n contains n
 *          copies of n; form the componentwise sum of the first n rows and
 *          concatenate these decimal numbers to get a(n).
 * https://oeis.org/A210257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of componentwise sums of the centered
 *            triangle where row r contains r copies of r; the sums are taken
 *            over columns offset -(n‑1)…(n‑1). The result is formed by
 *            concatenating the decimal representations of the column sums
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   digit_count_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_count_local.h"

static seq_t pow10_local(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

/* Main sequence function.  idx_t is uint64_t; seq_t may be any unsigned
   integer type or arbitrary‑precision integer. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* Undefined for n = 0, return 0 */

    /* Pre‑compute the sums for each column offset k = |c|.  The sum for
       a column with offset k (0 ≤ k < n) is
          m = floor((n - (k+1)) / 2)
          sum = (m+1) * (k+1 + m)                                       */
    seq_t result = (seq_t)0;

    /* Process left side (k from n-1 down to 1). */
    for (idx_t k = n - (idx_t)1; k > (idx_t)0; --k) {
        idx_t t = n - (k + (idx_t)1);
        idx_t m = t / (idx_t)2;
        seq_t sum = (seq_t)(m + (idx_t)1) * (seq_t)(k + (idx_t)1 + m);

        int d = digit_count_local(sum);
        result = result * pow10_local((idx_t)d) + sum;
    }

    /* Center column k = 0. */
    {
        idx_t t = n - (idx_t)1;
        idx_t m = t / (idx_t)2;
        seq_t sum = (seq_t)(m + (idx_t)1) * (seq_t)(m + (idx_t)1);   /* k = 0 */

        int d = digit_count_local(sum);
        result = result * pow10_local((idx_t)d) + sum;
    }

    /* Process right side (k from 1 to n-1). */
    for (idx_t k = (idx_t)1; k < n; ++k) {
        idx_t t = n - (k + (idx_t)1);
        idx_t m = t / (idx_t)2;
        seq_t sum = (seq_t)(m + (idx_t)1) * (seq_t)(k + (idx_t)1 + m);

        int d = digit_count_local(sum);
        result = result * pow10_local((idx_t)d) + sum;
    }

    return result;
}
