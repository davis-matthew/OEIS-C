/*
 * A209043  Number of nX4 0..3 arrays with no element equal the average of
 *          immediate neighbors vertically above and horizontally left of it.
 * https://oeis.org/A209043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries in {0,1,2,3}, where
 *            each element is not equal to the average of its existing
 *            immediate vertical‑above and horizontal‑left neighbors
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the first row only the left neighbor exists; for the first column only the
   above neighbor exists; for the remaining cells both neighbors exist and the
   average is (above+left)/2.  An element is forbidden if it equals that average
   (when the average is an integer).                                                         */

#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static uint8_t transition[256][256];
static uint8_t valid_first_row[256];

static void init(void)
{
    if (initialized) return;
    initialized = true;

    for (int p = 0; p < 256; ++p) {
        /* compute first row validity (only left neighbor) */
        bool ok = true;
        int prev = (p >> 0) & 3;          /* column 0 */
        for (int j = 1; j < 4; ++j) {
            int cur = (p >> (j << 1)) & 3;
            if (cur == prev) { ok = false; break; }
            prev = cur;
        }
        valid_first_row[p] = ok ? 1 : 0;

        for (int c = 0; c < 256; ++c) {
            bool v = true;
            /* column 0: only above neighbor */
            if (((c >> 0) & 3) == ((p >> 0) & 3)) { v = false; }
            /* columns 1..3: both neighbors exist */
            for (int j = 1; j < 4 && v; ++j) {
                int above = (p >> (j << 1)) & 3;
                int left  = (c >> ((j - 1) << 1)) & 3;
                int cell  = (c >> (j << 1)) & 3;
                int sum   = above + left;
                if ((sum & 1) == 0 && cell == (sum >> 1)) { v = false; }
            }
            transition[p][c] = v ? 1 : 0;
        }
    }
}

seq_t a(idx_t n)
{
    init();
    if (n == 0) return 0;

    static seq_t dp[256];
    static seq_t next[256];
    /* first row */
    for (int i = 0; i < 256; ++i) {
        dp[i] = valid_first_row[i] ? 1 : 0;
    }

    for (idx_t row = 2; row <= n; ++row) {      /* rows 2..n */
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int p = 0; p < 256; ++p) {
            seq_t val = dp[p];
            if (val == 0) continue;
            for (int c = 0; c < 256; ++c) {
                if (transition[p][c]) next[c] += val;
            }
        }
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += dp[i];
    return total;
}
