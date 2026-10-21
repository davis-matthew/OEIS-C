/*
 * A235574  Number of (n+1)X(1+1) 0..3 arrays with the difference of the upper
 *          and lower median value of each 2X2 subblock in lexicographically
 *          nondecreasing order rowwise and columnwise.
 * https://oeis.org/A235574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = number of (n+1) × 2 arrays with entries 0..3 such that
   for every 2×2 subblock the difference between upper and lower
   median values is nondecreasing when read rowwise.
   Computed as a dynamic‑programming over rows. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined below offset */

    /* Precompute differences for all 16 row states. */
    static unsigned char diff[16][16];
    static bool diff_inited = false;
    if (!diff_inited) {
        diff_inited = true;
        for (int r = 0; r < 16; ++r) {
            unsigned ar = r >> 2, br = r & 3;
            for (int s = 0; s < 16; ++s) {
                unsigned as = s >> 2, bs = s & 3;
                unsigned v[4] = { ar, br, as, bs };
                /* simple 4‑element insertion sort */
                for (int i = 1; i < 4; ++i) {
                    unsigned key = v[i], j = i - 1;
                    while (j < (unsigned)(-1) && v[j] > key) { v[j + 1] = v[j]; if (j == 0) { j = (unsigned)(-1); break; } else --j; }
                    v[j + 1] = key;
                }
                diff[r][s] = (unsigned char)(v[2] - v[1]);       /* upper - lower median */
            }
        }
    }

    /* DP arrays: dp_prev[row][diff] and dp_curr[row][diff]. */
    seq_t dp_prev[16][4];
    seq_t dp_curr[16][4];

    /* initialise with all 2‑row arrays (n = 1). */
    for (int r = 0; r < 16; ++r)
        for (int d = 0; d < 4; ++d)
            dp_prev[r][d] = 0;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            dp_prev[r1][diff[r0][r1]] += (seq_t)1;

    /* Add remaining rows. */
    idx_t rows = n + 1;                     /* total number of rows */
    idx_t steps = (rows > 2) ? rows - 2 : 0;   /* number of transitions beyond the first pair */

    for (idx_t step = 0; step < steps; ++step) {
        /* zero dp_curr */
        for (int r = 0; r < 16; ++r)
            for (int d = 0; d < 4; ++d)
                dp_curr[r][d] = 0;

        /* transition */
        for (int r_prev = 0; r_prev < 16; ++r_prev) {
            for (int d_prev = 0; d_prev < 4; ++d_prev) {
                seq_t cnt = dp_prev[r_prev][d_prev];
                if (cnt == 0) continue;
                for (int r_next = 0; r_next < 16; ++r_next) {
                    unsigned d_next = diff[r_prev][r_next];
                    if (d_next >= d_prev)
                        dp_curr[r_next][d_next] += cnt;
                }
            }
        }

        /* copy dp_curr to dp_prev for next iteration */
        for (int r = 0; r < 16; ++r)
            for (int d = 0; d < 4; ++d)
                dp_prev[r][d] = dp_curr[r][d];
    }

    /* sum all remaining states */
    seq_t result = 0;
    for (int r = 0; r < 16; ++r)
        for (int d = 0; d < 4; ++d)
            result += dp_prev[r][d];

    return result;
}
