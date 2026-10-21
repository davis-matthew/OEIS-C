/*
 * A060522  Number of 4 X n grids of black and white cells, no 3 of same color
 *          vertically or horizontally contiguous.
 * https://oeis.org/A060522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: Number of 4×n grids of black and white cells with no three
 * collinear cells of the same color (vertically or horizontally).
 * 
 *  a(n)  = #valid 4×n binary matrices avoiding runs of length ≥3
 *           in any row or column.
 *
 *  The calculation is done via a transfer‑matrix DP over column
 *  patterns.  There are 10 admissible 4‑bit column patterns (no run of
 *  three equal bits inside a column).  Abilities
 *  to form a new column depend only on the last two columns, so the
 *  state space is 10×10 = 100.  The DP iterates over the number of
 *  columns and counts all valid histories.  The complexity is
 *  O(n·100·10) ≈ 10⁷ operations for n ≈ 10⁴, well within the time
 *  limit.
 */

#include <stddef.h>   /* for idx_t, seq_t */

static const int ALLOWED_PATTERNS[10] = {2,3,4,5,6,9,10,11,12,13};
static int PAT_TO_INDEX[16] = {0};

static int TRANS[100][10];
static int TRANS_SZ[100];

/* initialise the static tables once */
static void init_tables(void)
{
    static int init = 0;
    if (init) return;
    init = 1;

    for (int i = 0; i < 16; ++i) PAT_TO_INDEX[i] = -1;
    for (int i = 0; i < 10; ++i) PAT_TO_INDEX[ALLOWED_PATTERNS[i]] = i;

    for (int p = 0; p < 10; ++p) {
        for (int q = 0; q < 10; ++q) {
            int idx = p * 10 + q;
            int sz = 0;
            for (int r = 0; r < 10; ++r) {
                int ok = 1;
                for (int row = 0; row < 4 && ok; ++row) {
                    int bp = (ALLOWED_PATTERNS[p] >> row) & 1;
                    int bq = (ALLOWED_PATTERNS[q] >> row) & 1;
                    int br = (ALLOWED_PATTERNS[r] >> row) & 1;
                    if (bp == bq && bq == br) ok = 0;
                }
                if (ok) TRANS[idx][sz++] = r;
            }
            TRANS_SZ[idx] = sz;
        }
    }
}

/* main function requested by the test harness */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;      /* undefined, return 0 for safety */
    if (n == 1) return 10;      /* one column: 10 admissible patterns */
    if (n == 2) return 100;     /* two columns: 10×10 */

    /* DP over lengths: dpPrev[state] counts histories ending with that state */
    static seq_t dpPrev[100], dpCurr[100];
    for (int i = 0; i < 100; ++i) dpPrev[i] = 1;   /* length 2, all pairs are valid */

    /* iterate from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero dpCurr */
        for (int i = 0; i < 100; ++i) dpCurr[i] = 0;
        for (int idx = 0; idx < 100; ++idx) {
            seq_t ways = dpPrev[idx];
            if (ways == 0) continue;
            int p = idx / 10;
            int q = idx % 10;
            for (int t = 0; t < TRANS_SZ[idx]; ++t) {
                int r = TRANS[idx][t];
                int nextIdx = q * 10 + r;
                dpCurr[nextIdx] += ways;
            }
        }
        /* swap dpPrev and dpCurr */
        for (int i = 0; i < 100; ++i) dpPrev[i] = dpCurr[i];
    }

    /* sum over all states for final length */
    seq_t total = 0;
    for (int i = 0; i < 100; ++i) total += dpPrev[i];
    return total;
}
