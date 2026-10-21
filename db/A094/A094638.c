/*
 * A094638  Triangle read by rows: T(n,k) = |s(n,n+1-k)|, where s(n,k) are the
 *          signed Stirling numbers of the first kind A008276 (1 <= k <= n; in
 *          other words, the unsigned Stirling numbers of the first kind in
 *          reverse order).
 * https://oeis.org/A094638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = the n‑th element of the triangle formed by the unsigned
 *            Stirling numbers of the first kind read by rows; i.e. a(n) =
 *            T(r,c) where r is the smallest integer with r*(r+1)/2 ≥ n, c = n
 *            - (r-1)*r/2, and T(r,c) = |s(r,r+1-c)|
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 100, u64 n <= 246,
 *            u128 n <= 623, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find row r such that cumulative count >= n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (n > cum) {
        ++r;
        cum += r;
    }
    /* Column index within this row. */
    idx_t col = n - (cum - r);          /* 1 ≤ col ≤ r */

    /* Compute unsigned Stirling numbers up to row r in place. */
    seq_t *c = (seq_t*)calloc(r + 1, sizeof(seq_t));
    if (!c) return 0;   /* allocation failure, unlikely for small r */

    c[0] = 1;   /* c(0,0) = 1 */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t k = row; k >= 1; --k) {
            c[k] = c[k - 1] + (seq_t)(row - 1) * c[k];
        }
        c[0] = 0;
    }

    /* Desired entry: c(r, r+1-col) */
    idx_t x = r + 1 - col;      /* 1 ≤ x ≤ r */
    seq_t result = c[x];

    free(c);
    return result;
}
