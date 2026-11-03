/*
 * A059832  A ternary tribonacci triangle: form the triangle as follows: start
 *          with 3 single values: 1, 2, 3. Each succeeding row is a
 *          concatenation of the previous 3 rows.
 * https://oeis.org/A059832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = A059825(n)+1
 * Range:     u8 n <= 30121, u16 n <= 30121, u32 n <= 30121, u64 n <= 30121,
 *            u128 n <= 30121, big any n
 * Validated: standard, pass, 121 terms through n = 9547 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of the ternary tribonacci triangle by
   locating the row that contains n and then recursing through
   the row construction rule. */

#include <stdint.h>

static seq_t row_value(int k, idx_t pos, uint64_t *len)
{
    if (k == 0) return (seq_t)1;
    if (k == 1) return (seq_t)2;
    if (k == 2) return (seq_t)3;
    /* Row k is the concatenation of row(k-3), row(k-2), row(k-1). */
    if (pos < len[k-3]) return row_value(k-3, pos, len);
    pos -= len[k-3];
    if (pos < len[k-2]) return row_value(k-2, pos, len);
    pos -= len[k-2];
    return row_value(k-1, pos, len);
}

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
