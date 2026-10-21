/*
 * A237922  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the lower
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A237922
   Diagram: Number of (n+1)×2 0..3 arrays with the maximum plus the lower
   median of every 2×2 subblock differing from its horizontal and vertical
   neighbours by exactly one.  This is implemented by dynamic programming
   on pairs of rows.  For each pair of consecutive rows (p,c) we compute
   v(p,c)=max+lower_median of the four cells.  For a valid array the
   values of successive subblocks must differ by exactly one, i.e.
   |v(p,c)-v(c,next)|==1.  The number of arrays equals the number of
   length‑n sequences of 16‑element row states satisfying this
   adjacency rule, which is obtained by iterating a 256‑state DP. */

#include <stddef.h>

#define STATE 256     /* 16 rows × 16 rows */

seq_t a(idx_t n) {
    /* Index counting */
    idx_t i, step;
    if (n < 1) return 0;  /* undefined for n<1, but safe */

    /* Pre‑computed data, constructed once */
    static int ready = 0;
    static unsigned char row_val[16][2];          /* each row: {top, bottom} */
    static int v[16][16];                         /* v[row1][row2] */
    static int next_count[STATE];
    static int next_list[STATE][16];
    static seq_t dp_curr[STATE], dp_next[STATE];

    if (!ready) {
        /* Unpack rows */
        for (i = 0; i < 16; ++i) {
            row_val[i][0] = (i >> 2) & 3;   /* top cell */
            row_val[i][1] = i & 3;          /* bottom cell */
        }

        /* Compute v values */
        for (i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j) {
                int a0 = row_val[i][0];
                int b0 = row_val[i][1];
                int a1 = row_val[j][0];
                int b1 = row_val[j][1];
                int vals[4] = {a0, b0, a1, b1};
                /* find max */
                int mx = vals[0];
                if (vals[1] > mx) mx = vals[1];
                if (vals[2] > mx) mx = vals[2];
                if (vals[3] > mx) mx = vals[3];
                /* find lower median (second smallest) */
                /* simple sort of four ints */
                /* insertion sort for 4 elements */
                int t;
                /* 0 <-> 1 */
                if (vals[0] > vals[1]) { t=vals[0]; vals[0]=vals[1]; vals[1]=t; }
                /* 0 <-> 2 */
                if (vals[0] > vals[2]) { t=vals[0]; vals[0]=vals[2]; vals[2]=t; }
                /* 1 <-> 2 */
                if (vals[1] > vals[2]) { t=vals[1]; vals[1]=vals[2]; vals[2]=t; }
                /* 0 <-> 3 */
                if (vals[0] > vals[3]) { t=vals[0]; vals[0]=vals[3]; vals[3]=t; }
                /* 1 <-> 3 */
                if (vals[1] > vals[3]) { t=vals[1]; vals[1]=vals[3]; vals[3]=t; }
                /* 2 <-> 3 */
                if (vals[2] > vals[3]) { t=vals[2]; vals[2]=vals[3]; vals[3]=t; }
                /* now vals[0] <= vals[1] <= vals[2] <= vals[3] */
                int lower_mid = vals[1];
                v[i][j] = mx + lower_mid;
            }

        /* Build adjacency lists for DP transitions */
        for (i = 0; i < STATE; ++i) {
            int pi = i / 16;
            int ci = i % 16;
            int count = 0;
            int vpc = v[pi][ci];
            for (idx_t nxt = 0; nxt < 16; ++nxt) {
                int vcn = v[ci][nxt];
                if (vpc + 1 == vcn || vpc - 1 == vcn) {
                    if (count < 16) {   /* safety, but at most 16 */
                        next_list[i][count++] = ci * 16 + (int)nxt;
                    }
                }
            }
            next_count[i] = count;
        }
        ready = 1;
    }

    /* DP initialisation: all 256 pairs possible for first subblock */
    for (i = 0; i < STATE; ++i) dp_curr[i] = 1;
    for (i = 0; i < STATE; ++i) dp_next[i] = 0;

    /* Perform n-1 transitions */
    for (step = 1; step < n; ++step) {
        /* reset dp_next */
        for (i = 0; i < STATE; ++i) dp_next[i] = 0;

        for (i = 0; i < STATE; ++i) {
            seq_t val = dp_curr[i];
            if (val == 0) continue;
            int cnt = next_count[i];
            for (int k = 0; k < cnt; ++k) {
                int ns = next_list[i][k];
                dp_next[ns] += val;
            }
        }
        /* swap dp_curr and dp_next */
        for (i = 0; i < STATE; ++i) dp_curr[i] = dp_next[i];
    }

    /* sum all DP states to get a(n) */
    seq_t total = 0;
    for (i = 0; i < STATE; ++i) total += dp_curr[i];
    return total;
}
