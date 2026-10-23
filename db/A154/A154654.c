/*
 * A154654  Number of permutations of length n within distance 5.
 * https://oeis.org/A154654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the permanent of the n×n matrix with 1's on the
 * eleven central diagonals.  The value can be computed by a linear
 * dynamic‑programming walk over the rows, keeping a 11‑bit mask that
 * records which of the 11 columns in the current “band” are already
 * occupied.  At step i (row i) the window is [i‑5,i+5]; the bit
 * corresponding to column j is (j‑i)+5.  After assigning column j,
 * column i‑5 must already be matched, otherwise the state is
 * invalid.  The mask is then shifted right by one bit to prepare for
 * the next row.  At the final step all columns 1…n must be matched,
 * which means that bits 0…5 of the final mask are all 1.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* 11‑bit masks (0 … 2047) */
    static seq_t dp[2048];
    static seq_t nextdp[2048];
    size_t i, mask, j;
    seq_t ans = (seq_t)0;

    /* initialise DP */
    for (mask = 0; mask < 2048; ++mask) dp[mask] = (seq_t)0;
    dp[31] = (seq_t)1;                    /* bits 0–4 set (columns < 1) */

    for (idx_t row = 1; row <= n; ++row) {
        for (mask = 0; mask < 2048; ++mask) nextdp[mask] = (seq_t)0;

        /* allowed column range for this row */
        idx_t start = (row > 5 ? row - 5 : (idx_t)1);
        idx_t end   = (row + 5 <= n ? row + 5 : n);

        for (mask = 0; mask < 2048; ++mask) {
            seq_t val = dp[mask];
            if (val == (seq_t)0) continue;

            for (j = start; j <= end; ++j) {
                int   off = (int)(j - row);          /* -5 … +5 */
                unsigned bit = (unsigned)(off + 5);

                if ((mask & (1u << bit)) != 0) continue;   /* already used */

                unsigned new_mask = mask | (1u << bit);

                if (row < n) {
                    if ((new_mask & 1u) == 0) continue;     /* column row-5 not yet matched */
                    unsigned next_mask = (new_mask >> 1) & 0x7FFu; /* keep 11 bits */
                    nextdp[next_mask] += val;
                } else { /* last row: check all real columns matched */
                    if ((new_mask & 0x3Fu) == 0x3Fu)          /* bits 0–5 all 1 */
                        ans += val;
                }
            }
        }

        /* swap dp and nextdp */
        for (mask = 0; mask < 2048; ++mask) dp[mask] = nextdp[mask];
    }

    return ans;
}
