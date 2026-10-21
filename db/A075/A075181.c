/*
 * A075181  Coefficients of certain polynomials (rising powers).
 * https://oeis.org/A075181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n,m) = (n-m)*a(n-1,m)+(n-1)*a(n-1,m-1), a(1,0)=1. The sequence
 *            a(k) enumerates the entries a(n,m) row‑by‑row (m increases from
 *            0 to n-1 for each row n). This implementation converts the flat
 *            index k into the corresponding row n and column m and computes
 *            a(n,m) by a simple recurrence. The maximu...
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 68, u64 n <= 175,
 *            u128 n <= 473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n */
    idx_t r = (idx_t)((sqrtl((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    if (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Column index m (0‑based) within row r */
    idx_t m = n - (r - 1) * r / 2 - 1;
    if (r == 1) {                  /* row 1 is just 1 */
        return (seq_t)1;
    }

    /* Two buffers large enough for the maximum needed row */
    seq_t prev[2000];
    seq_t cur[2000];

    prev[0] = 1;                    /* a(1,0) = 1 */

    /* Build rows incrementally up to row r */
    for (idx_t row = 2; row <= r; ++row) {
        for (idx_t j = 0; j < row; ++j) {
            seq_t t1 = 0, t2 = 0;
            if (j < row - 1) {      /* a(row-1,j) exists */
                t1 = (seq_t)(row - j) * prev[j];
            }
            if (j > 0) {            /* a(row-1,j-1) exists */
                t2 = (seq_t)(row - 1) * prev[j - 1];
            }
            cur[j] = t1 + t2;
        }
        /* Copy current row into prev for next iteration */
        for (idx_t j = 0; j < row; ++j) {
            prev[j] = cur[j];
        }
    }

    return prev[m];
}
