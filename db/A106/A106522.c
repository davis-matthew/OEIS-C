/*
 * A106522  A Pascal type matrix based on the tribonacci numbers.
 * https://oeis.org/A106522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 174, u32 n <= 606, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106522: enumerate the tribonacci‑based Pascal triangle
 * row by row.
 *
 * Triangle definition:
 *   T(0,0) = trib(2)
 *   T(n,0) = trib(n+2)            (n ≥ 1)
 *   T(n,k) = T(n-1,k-1) + T(n-1,k) (1 ≤ k ≤ n)
 *
 * The function returns the n‑th entry (0‑based) of this triangle.
 */
#include <stddef.h>  /* for size_t in malloc prototypes if needed */

/* maximum row needed for n ≤ 10000 (row 141) – a small safety margin */
#define MAX_ROW 200

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) such that n lies in that row */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* Pre‑compute tribonacci numbers up to trib(r+2) */
    seq_t trib[MAX_ROW + 3];
    trib[0] = 0; trib[1] = 0; trib[2] = 1;
    for (idx_t i = 3; i <= r + 2; ++i)
        trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];

    /* Build rows of the triangle sequentially up to row r */
    seq_t prev[MAX_ROW + 1];
    seq_t cur[MAX_ROW + 1];

    /* Row 0 */
    prev[0] = trib[2];
    if (r == 0)
        return prev[0];

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        cur[0] = trib[i + 2];            /* first element of the row */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t v = prev[j - 1];
            if (j <= i - 1)
                v += prev[j];
            cur[j] = v;
        }
        /* copy current row into prev for the next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = cur[j];
    }

    return prev[c];
}
