/*
 * A137650  Triangle read by rows, A008277 * A000012.
 * https://oeis.org/A137650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=p}^{r} S(r,k) where r is the minimal integer with
 *            r(r+1)/2 ≥ n, p = n - r(r-1)/2, and S(r,k) are Stirling numbers
 *            of the second kind
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n < 1, return 0 */

    /* Find row r: minimal r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) r++;  /* r increases by 1 each iteration */
    r = (r + 1);  /* after loop r is the smallest such that r*(r+1)/2 ≥ n */

    /* Position inside the row (1‑based) */
    idx_t prev_cnt = (r - 1) * r / 2;
    idx_t p = n - prev_cnt;   /* 1 <= p <= r */

    /* Allocate temporary arrays for Stirling numbers of second kind */
    idx_t size = r + 1;
    seq_t *prev = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(size, sizeof(seq_t));

    prev[0] = 1;  /* S(0,0) = 1 */

    /* DP to compute row r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = 0;   /* S(i,0) = 0 for i > 0 */
        for (idx_t k = 1; k <= i; ++k) {
            curr[k] = prev[k] * (seq_t)k + prev[k - 1];
        }
        /* swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the required partial sum from right (i.e., from k=p to r) */
    seq_t sum = 0;
    for (idx_t k = r; ; --k) {
        sum += prev[k];
        if (k == p) break;
    }

    /* Clean up */
    free(prev);
    free(curr);

    return sum;
}
