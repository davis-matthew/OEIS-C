/*
 * A210586  Triangle T(n,k) read by rows: T(n,k) is the number of rooted
 *          hypertrees on n labeled vertices with k hyperedges, n >= 2, k >=
 *          1.
 * https://oeis.org/A210586
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = n^k * Stirling2(n‑1,k) – the sequence a(n) is the *
 *            triangle read by rows: a(2)=T(2,1), a(3)=T(3,1), a(4)=T(3,2), *
 *            a(5)=T(4,1), …
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 43, u64 n <= 116, u128 n <= 341,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* maximum number of vertices needed for indices up to 10 000
 * (row 142 contains 141 elements, cumulative 10 071 > 10 000) */
#define MAXV 150

static seq_t S[MAXV + 1][MAXV + 1];
static bool init_done = false;

/* precompute Stirling numbers of the second kind up to MAXV */
static void init_stirling(void) {
    if (init_done) return;

    for (idx_t i = 0; i <= MAXV; ++i)
        for (idx_t j = 0; j <= MAXV; ++j)
            S[i][j] = 0;

    S[0][0] = 1;
    for (idx_t n = 1; n <= MAXV; ++n) {
        S[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            S[n][k] = (seq_t)k * S[n - 1][k] + S[n - 1][k - 1];
        }
    }
    init_done = true;
}

/* a(n): flattening of the triangle by rows */
seq_t a(idx_t n) {
    if (n < 2) return 0;           /* offset starts at 2 */

    init_stirling();

    idx_t offset = n - 2;          /* zero‑based offset in flattened list */

    /* find row m such that offset < m*(m-1)/2 */
    idx_t m = 2;
    idx_t cum = 1; /* terms up to row m (inclusive) */
    while (offset >= cum) {
        ++m;
        cum += m - 1;
    }

    /* terms before this row */
    idx_t prev_cum = cum - (m - 1);
    idx_t k = offset - prev_cum + 1; /* 1‑based column index */

    /* compute m^k */
    seq_t pow = 1;
    seq_t base = (seq_t)m;
    for (idx_t i = 0; i < k; ++i) {
        pow = pow * base;
    }

    /* T(m,k) = m^k * Stirling2(m-1,k) */
    seq_t result = pow * S[m - 1][k];
    return result;
}
