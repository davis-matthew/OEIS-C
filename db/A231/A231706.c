/*
 * A231706  Number of (n+1)X(4+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×5 binary arrays with no strict majority among
 *            horizontal, vertical, and anti-diagonal neighbors. Computed via
 *            DP over rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   row_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_is_valid.h"

/* Pre‑computed validity tables.  They are built only once. */
static bool precomp_done = false;
static bool top_valid[32][32];      /* above absent, below present     */
static bool bottom_valid[32][32];   /* above present, below absent     */
static bool mid_valid[32][32][32];  /* both neighbours present         */

static void precompute_validity(void)
{
    if (precomp_done) return;
    precomp_done = true;

    for (uint8_t r = 0; r < 32; ++r)
        for (uint8_t b = 0; b < 32; ++b)
            top_valid[r][b] = row_is_valid(0, false, r, b, true);

    for (uint8_t a = 0; a < 32; ++a)
        for (uint8_t r = 0; r < 32; ++r)
            bottom_valid[a][r] = row_is_valid(a, true, r, 0, false);

    for (uint8_t a = 0; a < 32; ++a)
        for (uint8_t r = 0; r < 32; ++r)
            for (uint8_t b = 0; b < 32; ++b)
                mid_valid[a][r][b] = row_is_valid(a, true, r, b, true);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* no calls with offset 1 */
    precompute_validity();

    const idx_t N = n;             /* N is number of rows minus 1 */

    /* dpFirst[a] – number of ways after only the first row a */
    seq_t dpFirst[32];
    for (uint8_t i = 0; i < 32; ++i) dpFirst[i] = 1;

    /* dpPairs[a][b] – number of ways after having rows ... a, b
       with a being the previous row and b the current last row. */
    seq_t dpPairs[32][32] = {{0}};
    seq_t dpNext[32][32];

    /* Add the second row (index 1).  Validate the first row. */
    for (uint8_t a = 0; a < 32; ++a)
        for (uint8_t b = 0; b < 32; ++b)
            if (top_valid[a][b]) dpPairs[a][b] = dpFirst[a];   /* +1 each */

    /* If we have only two rows, finish here. */
    if (N == 1) {
        seq_t ans = 0;
        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b)
                if (bottom_valid[a][b]) ans += dpPairs[a][b];
        return ans;
    }

    /* For rows 2 … N we iterate, checking the middle rows. */
    for (idx_t i = 2; i <= N; ++i) {
        /* zero dpNext */
        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b)
                dpNext[a][b] = 0;

        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b) {
                seq_t cur = dpPairs[a][b];
                if (cur == 0) continue;
                for (uint8_t c = 0; c < 32; ++c)
                    if (mid_valid[a][b][c]) dpNext[b][c] += cur;
            }

        /* copy dpNext into dpPairs for the next iteration */
        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b)
                dpPairs[a][b] = dpNext[a][b];
    }

    /* Final check – validate the last row (no below neighbour). */
    seq_t ans = 0;
    for (uint8_t a = 0; a < 32; ++a)
        for (uint8_t b = 0; b < 32; ++b)
            if (bottom_valid[a][b]) ans += dpPairs[a][b];

    return ans;
}
