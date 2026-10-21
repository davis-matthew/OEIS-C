/*
 * A203938  Number of (n+1)X3 0..5 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×3 arrays of digits 0..5 where * (i) for every
 *            row i, the horizontal pair sums * a(i,0)+a(i,1) and
 *            a(i,1)+a(i,2) are nondecreasing downward * (ii) for every column
 *            j, the vertical pair sums * a(i,j)+a(i-1,j) are nondecreasing
 *            across columns. * This is implemented by dynamic progr...
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 20, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* external integer types are given by the build */
seq_t a(idx_t n)
{
    /* Pre‑computed data, initialized once */
    static int p0[216], p1[216], p2[216];
    static int next[216][216];
    static int next_len[216];
    static int init_done = 0;

    if (!init_done) {
        /* Enumerate all 3‑digit patterns (0..5) */
        for (int idx = 0; idx < 216; ++idx) {
            p0[idx] = idx / 36;          /* a0 */
            p1[idx] = (idx / 6) % 6;     /* a1 */
            p2[idx] = idx % 6;           /* a2 */
        }
        /* Build transition lists:
           next[prev] = list of cur indices allowed after prev. */
        for (int prev = 0; prev < 216; ++prev) {
            int a0 = p0[prev];
            int a1 = p1[prev];
            int a2 = p2[prev];
            int sum01 = a0 + a1;
            int sum12 = a1 + a2;
            int len = 0;
            for (int cur = 0; cur < 216; ++cur) {
                int b0 = p0[cur];
                int b1 = p1[cur];
                int b2 = p2[cur];
                if (b0 + b1 < sum01) continue;          /* horizontal down */
                if (b1 + b2 < sum12) continue;
                if (b0 + a0 > b1 + a1) continue;        /* vertical right */
                if (b1 + a1 > b2 + a2) continue;
                next[prev][len++] = cur;
            }
            next_len[prev] = len;
        }
        init_done = 1;
    }

    /* Dynamic programming over rows */
    static seq_t dp_prev[216];
    static seq_t dp_next[216];

    /* Initial row: any of the 216 patterns */
    for (int i = 0; i < 216; ++i) dp_prev[i] = (seq_t)1;

    /* Number of transitions = number of rows – 1 = n (since n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        /* reset next layer */
        for (int i = 0; i < 216; ++i) dp_next[i] = 0;
        /* propagate */
        for (int prev = 0; prev < 216; ++prev) {
            seq_t val = dp_prev[prev];
            if (val == 0) continue;
            int len = next_len[prev];
            for (int k = 0; k < len; ++k) {
                int cur = next[prev][k];
                dp_next[cur] += val;
            }
        }
        /* swap layers */
        for (int i = 0; i < 216; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum all possibilities for the final row */
    seq_t total = 0;
    for (int i = 0; i < 216; ++i) total += dp_prev[i];
    return total;
}
