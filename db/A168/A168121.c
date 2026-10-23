/*
 * A168121  Triangle T(n,k) read by rows in which column k lists each number
 *          A000009 followed by k-1 zeros, for k>0.
 * https://oeis.org/A168121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A000009[r / c] if r % c == 0, else 0, where r is the
 *            unique integer with r(r+1)/2 ≥ n and c = n - r(r-1)/2. A000009
 *            is the partition‑into‑distinct‑parts sequence
 *            (1,1,2,2,3,4,5,6,…)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute A000009 up to 200.  This is more than enough
       for n up to 10 000 (row number ~141). */
    static seq_t a0[201];
    static int inited = 0;
    if (!inited) {
        const idx_t MAXR = 200;
        seq_t dp[201];
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAXR; ++i) dp[i] = (seq_t)0;
        for (idx_t part = 1; part <= MAXR; ++part) {
            for (idx_t sum = MAXR; sum >= part; --sum) {
                dp[sum] = dp[sum] + dp[sum - part];
            }
        }
        for (idx_t i = 1; i <= MAXR; ++i) a0[i] = dp[i];
        inited = 1;
    }

    /* Find the row r that contains position n.  The triangular
       numbers give the cumulative counts. */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index c (1‑based) */
    idx_t start = r * (r - 1) / 2;
    idx_t c = n - start;

    /* Return the desired value. */
    if (r % c == 0) {
        idx_t idx = r / c;          /* 1‑based index into A000009 */
        return a0[idx];
    } else {
        return (seq_t)0;
    }
}
