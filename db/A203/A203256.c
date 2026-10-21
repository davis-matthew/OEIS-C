/*
 * A203256  Number of nX2 0..7 arrays with every nonzero element less than or
 *          equal to some NW, E or S neighbor.
 * https://oeis.org/A203256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203256: Number of n×2 arrays 0..7 with every nonzero element
   <= at least one of its NW, E or S neighbors.
   Computation method: dynamic programming by rows.
   State: for the current row we store its two cell values (0..7)
          and two flags indicating whether each cell still needs a South
          neighbor to satisfy the condition.
   Transition: when a new row is added, any pending need from the
          previous row must be met by the current row's same‑column value.
          The new state’s need flags are computed from the current
          row values and their left/right/West (NW) neighbors.       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* States: 8 (values) * 8 (values) * 2 (need0) * 2 (need1) = 256 */
    seq_t dp_cur[256] = {0};
    seq_t dp_next[256];

    /* ---------- Initial row (row 0) ----------
       For each (a,b) compute need flags for the next row. */
    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            int need0 = (a != 0 && b < a) ? 1 : 0;    /* E neighbor */
            int need1 = (b != 0) ? 1 : 0;              /* only S possible on first row */
            int state = (a << 5) | (b << 2) | (need0 << 1) | need1;
            dp_cur[state] += (seq_t)1;
        }
    }

    /* ---------- Process rows 1 .. n-1 ----------
       For each state, try all possible next row values. */
    for (idx_t row = 1; row < n; ++row) {
        /* clear dp_next */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        for (int s = 0; s < 256; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == 0) continue;

            int p0 = (s >> 5) & 7;      /* previous row left value */
            int p1 = (s >> 2) & 7;      /* previous row right value */
            int need0 = (s >> 1) & 1;   /* pending need for left cell */
            int need1 = s & 1;          /* pending need for right cell */

            for (int a = 0; a < 8; ++a) {
                if (need0 && a < p0) continue;   /* must satisfy left need */
                for (int b = 0; b < 8; ++b) {
                    if (need1 && b < p1) continue; /* must satisfy right need */

                    /* Compute new need flags for current row */
                    int need0p = (a != 0 && b < a) ? 1 : 0;       /* E neighbor */
                    int need1p = (b != 0 && p0 < b) ? 1 : 0;       /* NW neighbor */

                    int ns = (a << 5) | (b << 2) | (need0p << 1) | need1p;
                    dp_next[ns] += cnt;
                }
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* ---------- Sum valid final states ----------
       All final pending needs must be zero. */
    seq_t ans = 0;
    for (int s = 0; s < 256; ++s) {
        if ( ((s >> 1) & 1) == 0 && (s & 1) == 0 ) {
            ans += dp_cur[s];
        }
    }
    return ans;
}
