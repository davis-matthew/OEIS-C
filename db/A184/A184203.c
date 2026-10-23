/*
 * A184203  Number of (n+1) X 6 binary arrays with no 2 X 2 subblock
 *          containing fewer than two 1's.
 * https://oeis.org/A184203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 binary arrays with no 2×2 subblock that
 *            contains fewer than two 1's. Computed by dynamic programming
 *            over the 64 possible row patterns
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Build the adjacency table of compatible rows once. */
static bool allowed_built = false;
static bool allowed[64][64];

/* allowed[j][k] == true  if rows j (above) and k (below) form a
   valid 2×2 block at every column pair. */
static void build_allowed(void)
{
    if (allowed_built) return;
    for (int j = 0; j < 64; ++j) {
        for (int k = 0; k < 64; ++k) {
            bool ok = true;
            /* 6 columns → 5 overlapping 2×2 blocks */
            for (int c = 0; c < 5 && ok; ++c) {
                int b00 = (j >> c) & 1;
                int b01 = (j >> (c + 1)) & 1;
                int b10 = (k >> c) & 1;
                int b11 = (k >> (c + 1)) & 1;
                int sum = b00 + b01 + b10 + b11;
                if (sum < 2) ok = false;
            }
            allowed[j][k] = ok;
        }
    }
    allowed_built = true;
}

/* a(n) for n ≥ 1 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0 */

    build_allowed();

    /* DP state for the number of arrays ending with each row pattern */
    seq_t dp_prev[64];
    seq_t dp_cur[64];

    /* Initial state: one 1×6 array for each possible first row */
    for (int i = 0; i < 64; ++i)
        dp_prev[i] = (seq_t)1;

    /* Add rows one by one */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero dp_cur */
        for (int i = 0; i < 64; ++i)
            dp_cur[i] = (seq_t)0;

        /* propagate along allowed transitions */
        for (int j = 0; j < 64; ++j) {
            seq_t v = dp_prev[j];
            if (v == (seq_t)0) continue;
            for (int k = 0; k < 64; ++k) {
                if (allowed[j][k])
                    dp_cur[k] += v;
            }
        }
        /* move dp_cur into dp_prev for next iteration */
        for (int i = 0; i < 64; ++i)
            dp_prev[i] = dp_cur[i];
    }

    /* sum over all patterns to get the total number of arrays */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 64; ++i)
        sum += dp_prev[i];

    return sum;
}
