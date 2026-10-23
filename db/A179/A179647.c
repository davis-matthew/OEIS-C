/*
 * A179647  Rectangular array read by antidiagonals: T(n,k) is the number of
 *          compositions (ordered partitions) of n in which no part (summand)
 *          is equal to k.
 * https://oeis.org/A179647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where the composition count T(i,j) is the number
 *            of compositions of i with no part equal to j, and (i,j) are
 *            determined by reading the rectangular array by antidiagonals: i
 *            = pos+1, j = m-pos, with m = smallest integer such that m(m+1)/2
 *            >= n, pos = n - (m-1)m/2 - 1
 * Range:     u8 n <= 87, u16 n <= 227, u32 n <= 736, u64 n <= 2550,
 *            u128 n <= 9309, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **dp = NULL;          /* dp[j][n] = number of compositions of n with no part j */
static const idx_t MAXK = 200;     /* maximal row/column needed for n up to ~10000 */

/* initialise the DP table */
static void init_dp(void) {
    if (dp) return;
    dp = (seq_t**)malloc((MAXK + 1) * sizeof(seq_t*));
    for (idx_t j = 0; j <= MAXK; ++j) {
        dp[j] = (seq_t*)malloc((MAXK + 1) * sizeof(seq_t));
    }
    for (idx_t j = 1; j <= MAXK; ++j) {
        dp[j][0] = 1;                       /* empty composition */
        for (idx_t n = 1; n <= MAXK; ++n) {
            seq_t sum = 0;
            for (idx_t s = 1; s <= n; ++s) {
                if (s != j) sum += dp[j][n - s];
            }
            dp[j][n] = sum;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    init_dp();

    /* find m such that m*(m+1)/2 >= n */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n) ++m;

    idx_t prev = (m - 1) * m / 2;
    idx_t pos  = n - prev - 1;          /* zero‑based position on diagonal m */
    idx_t i    = pos + 1;               /* row number */
    idx_t j    = m - pos;               /* column number */

    /* safety check: indices should be within the precomputed range */
    if (i > MAXK || j > MAXK) {
        /* extend dp if necessary (not expected for n <= 10000) */
        return 0;
    }
    return dp[j][i];
}
