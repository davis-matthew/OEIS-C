/*
 * A206876  Number of 7Xn 0..1 arrays avoiding 0 0 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A206876:
   Number of 7×n binary arrays avoiding horizontally the pattern 0 0 0
   and vertically the pattern 1 0 1.
   The algorithm keeps the last two columns (each a 7‑bit mask) and
   transitions to a new column if it satisfies the vertical constraint
   and does not create three consecutive zeros in any row.
   For each possible previous column p the counts of all previous‑previous
   columns pp are aggregated by a subset zeta transform, enabling a
   transition in O(128²) per column.  For n ≤ 10000 the method runs in
   well under the time limit. */

#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    /* ---- Pre‑compute allowed columns (no vertical 1 0 1) ---- */
    static bool allowed[128];
    static int  alist[128];
    static int  aclen = -1;
    if (aclen < 0) {
        aclen = 0;
        for (int col = 0; col < 128; ++col) {
            bool ok = true;
            /* forbid 1 0 1 in any 3‐consecutive rows */
            for (int i = 0; i <= 4; ++i) {
                int b0 = (col >> i)     & 1;
                int b1 = (col >> (i+1)) & 1;
                int b2 = (col >> (i+2)) & 1;
                if (b0 == 1 && b1 == 0 && b2 == 1) { ok = false; break; }
            }
            allowed[col] = ok;
            if (ok) alist[aclen++] = col;
        }
    }

    /* ---- DP tables : dp[cur/next][prevprev][prev] ---- */
    static seq_t dp[2][128][128];
    seq_t (*cur)[128] = dp[0];
    seq_t (*nxt)[128] = dp[1];

    /* Initialise: everything zero, except state (127,127)=1 */
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j)
            cur[i][j] = 0;
    cur[127][127] = 1;

    /* Temporary storage for zeta transform */
    seq_t super[128];
    seq_t cnt[128];

    /* ---- Iterate over columns ---- */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next table */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                nxt[i][j] = 0;

        /* For each possible previous column p */
        for (int p = 0; p < 128; ++p) {

            /* Copy counts of all prevprev for this p */
            for (int pp = 0; pp < 128; ++pp)
                cnt[pp] = cur[pp][p];

            /* Initialise super with cnt */
            for (int m = 0; m < 128; ++m)
                super[m] = cnt[m];

            /* Subset zeta transform: super[mask] = sum over pp ⊇ mask of cnt[pp] */
            for (int bit = 0; bit < 7; ++bit) {
                int b = 1 << bit;
                for (int m = 0; m < 128; ++m)
                    if ((m & b) == 0)
                        super[m] += super[m | b];
            }

            /* Transition to all allowed new columns c */
            for (int k = 0; k < aclen; ++k) {
                int c = alist[k];
                int req = ((~c) & (~p)) & 127;   /* positions where both pp and p are 0 */
                seq_t v = super[req];
                if (v != 0)                /* add to state (p,c) */
                    nxt[p][c] += v;
            }
        }

        /* swap current and next */
        seq_t (*tmp)[128] = cur; cur = nxt; nxt = tmp;
    }

    /* ---- Sum all ending states ---- */
    seq_t total = 0;
    for (int pp = 0; pp < 128; ++pp)
        for (int p = 0; p < 128; ++p)
            total += cur[pp][p];

    return total;
}
