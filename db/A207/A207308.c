/*
 * A207308  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   horizontal_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207308: dynamic‑programming on columns.
   a(1) is the number of binary columns of height 5 that avoid
   the vertical patterns 001 and 110.  For n ≥ 2 the value counts
   5×n binary arrays that avoid the horizontal patterns 000 and 101
   in every row and the vertical patterns 001 and 110 in every
   column.  The program builds transition tables only once and
   then uses an O(n⋅M³) DP where M = 10.  No lookup table is used. */

#include <stdbool.h>
#include "../utils/horizontal_allowed.h"

/* --- Transition tables (initialized on first call) --- */
static bool ready = false;          /* whether the tables are ready    */
static const int MAXB = 32;          /* 5‑bit columns → 32 possible    */
static int M;                        /* number of vertical‑valid cols  */
static int col_bits[32];             /* list of valid column patterns  */
static int trans_count[32][32];      /* # of allowed next columns      */
static int trans_next[32][32][32];   /* indices of allowed next cols   */

/* DP working arrays (size 32×32 is enough for all states). */
static seq_t dp_prev[32][32];
static seq_t dp_curr[32][32];

/* Check whether a 5‑bit column satisfies the vertical constraints
   (no sub‑triples equal to 001 or 110).  Row 0 is the LSB. */
static inline bool vertical_ok(int bits)
{
    for (int r = 0; r <= 2; ++r) {
        int tri = ((bits >> r) & 1) |
                  (((bits >> (r + 1)) & 1) << 1) |
                  (((bits >> (r + 2)) & 1) << 2);
        if (tri == 1 || tri == 6)        /* 001 or 110 */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Build tables on first use. */
    if (!ready) {
        M = 0;
        for (int bits = 0; bits < MAXB; ++bits)
            if (vertical_ok(bits)) {
                col_bits[M++] = bits;
            }

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                int cnt = 0;
                for (int k = 0; k < M; ++k) {
                    if (horizontal_allowed(col_bits[i], col_bits[j], col_bits[k])) {
                        trans_next[i][j][cnt++] = k;
                    }
                }
                trans_count[i][j] = cnt;
            }
        }
        ready = true;
    }

    /* n = 1: number of valid columns. n = 2: any pair works. */
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    /* DP for n ≥ 3.  dp_prev[i][j] holds counts of column pairs (i,j)
       for the current prefix. */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dp_prev[i][j] = 1;         /* initial pair counts */

    for (idx_t pos = 3; pos <= n; ++pos) {
        /* Zero previous work array. */
        for (int i = 0; i < M; ++i)
            for (int k = 0; k < M; ++k)
                dp_curr[i][k] = 0;

        for (int i = 0; i < M; ++i) {          /* column i (prev‑prev) */
            for (int k = 0; k < M; ++k) {      /* column k (prev) */
                seq_t val = dp_prev[i][k];
                if (!val) continue;

                int cnt = trans_count[i][k];
                int *nexts = trans_next[i][k];
                for (int t = 0; t < cnt; ++t) {
                    int nxt = nexts[t];
                    dp_curr[k][nxt] += val;
                }
            }
        }

        /* Copy back for next iteration. */
        for (int i = 0; i < M; ++i)
            for (int k = 0; k < M; ++k)
                dp_prev[i][k] = dp_curr[i][k];
    }

    /* Sum over all final column pairs. */
    seq_t total = 0;
    for (int i = 0; i < M; ++i)
        for (int k = 0; k < M; ++k)
            total += dp_prev[i][k];

    return total;
}
