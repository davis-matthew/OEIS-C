/*
 * A137510  Irregular triangle read by rows in which row n lists the divisors
 *          of n in the range 1 < d < n; or 0 if there are no such divisors.
 * https://oeis.org/A137510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the nth term of the flattened irregular triangle * where
 *            row k lists all divisors d of k with 1 < d < k in increasing
 *            order. * If k has no such divisors, row k contains a single 0
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t row = 1;      /* current row number */
    idx_t pos = n;      /* position to reach within the flattened list */

    /* Temporary buffers for divisors of the current row.
       2048 is ample for all rows encountered in the required range. */
    size_t small[2048];
    size_t high[2048];

    while (1) {
        /* Find all non‑trivial divisors of `row`. */
        size_t sc = 0;          /* count of small divisors (≤ sqrt(row))   */
        size_t hc = 0;          /* count of large divisors (> sqrt(row))  */

        for (idx_t d = 2; d * d <= row; ++d) {
            if (row % d == 0) {
                small[sc++] = (size_t)d;
                idx_t other = row / d;
                if (other != d)       /* avoid duplicate when row is a square */
                    high[hc++] = (size_t)other;
            }
        }

        size_t nd = sc + hc;          /* number of divisors (excluding 1 and row) */
        idx_t row_len = (nd ? nd : 1);/* length of this row in the flattened list */

        if (pos <= row_len) {
            /* Desired term lies in the current row. */
            if (nd == 0)                /* row has no non‑trivial divisors */
                return (seq_t)0;

            /* pos is 1‑based within the row. */
            if (pos <= sc)              /* in the small half */
                return (seq_t)small[pos - 1];
            else                         /* in the high half (reversed order) */
                return (seq_t)high[hc - (pos - sc)];
        }

        /* Skip this row and proceed to the next one. */
        pos -= row_len;
        ++row;
    }
}
