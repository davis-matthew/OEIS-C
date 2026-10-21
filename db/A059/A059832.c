/*
 * A059832  A ternary tribonacci triangle: form the triangle as follows: start
 *          with 3 single values: 1, 2, 3. Each succeeding row is a
 *          concatenation of the previous 3 rows.
 * https://oeis.org/A059832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = A059825(n)+1
 * Range:     u8 n <= 9547, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   row_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of the ternary tribonacci triangle by
   locating the row that contains n and then recursing through
   the row construction rule. */

#include <stdint.h>
#include "../utils/row_value.h"

seq_t a(idx_t n)
{
    /* Pre‑compute tribonacci row lengths until the cumulative sum
       exceeds n.  The maximum index needed for n up to ~10⁴ is < 20. */
    const int MAXK = 64;
    uint64_t len[MAXK];
    len[0] = len[1] = len[2] = 1;

    uint64_t cum = 0;
    int k = 0;
    for (;;)
    {
        if (k > 2 && k < MAXK)
            len[k] = len[k-1] + len[k-2] + len[k-3];

        if (cum + len[k] > n)
            break;

        cum += len[k];
        k++;
        if (k >= MAXK)
            break;   /* safety guard – for very large n */
    }

    /* Position of the required element within row k. */
    idx_t pos = n - cum;

    return row_value(k, pos, len);
}
