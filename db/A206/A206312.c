/*
 * A206312  Number of (n+1)X3 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A206312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the number of (n+1)×3 arrays of entries 0..3 that
   satisfy the conditions of OEIS A206312.  
   We use a transfer‑matrix over the 64 possible rows (3‑tuples from {0,1,2,3}):
   * each 2×2 subblock must have exactly two clockwise edge increases, and
   * its determinant must be non‑zero.
   The number of arrays of height L=n+1 equals the total number of
   length‑L paths in the state graph defined by these adjacency rules.  */
#include <stdbool.h>

static unsigned char allowed[64][64];   /* 1 if a transition is allowed, 0 else */
static bool initialized = false;

static void init_table(void)
{
    if (initialized) return;
    initialized = true;

    for (int p = 0; p < 64; ++p) {
        int p0 = (p >> 4) & 3;
        int p1 = (p >> 2) & 3;
        int p2 =  p       & 3;
        for (int c = 0; c < 64; ++c) {
            int c0 = (c >> 4) & 3;
            int c1 = (c >> 2) & 3;
            int c2 =  c       & 3;

            bool ok = true;

            /* first 2×2 subblock (columns 1‑2) */
            int tl = p0, tr = p1, bl = c0, br = c1;
            int inc = 0;
            if (tr > tl) ++inc;
            if (br > tr) ++inc;
            if (bl > br) ++inc;
            if (tl > bl) ++inc;
            if (inc != 2) ok = false;
            if (tl * br == tr * bl) ok = false;

            /* second 2×2 subblock (columns 2‑3) */
            if (ok) {
                tl = p1; tr = p2; bl = c1; br = c2;
                inc = 0;
                if (tr > tl) ++inc;
                if (br > tr) ++inc;
                if (bl > br) ++inc;
                if (tl > bl) ++inc;
                if (inc != 2) ok = false;
                if (tl * br == tr * bl) ok = false;
            }

            allowed[p][c] = ok ? 1 : 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, return 0 */
    init_table();

    /* dp_cur holds counts after processing i rows, i goes 1..(n+1) */
    static seq_t dp_cur[64];
    static seq_t dp_nxt[64];
    for (int i = 0; i < 64; ++i) dp_cur[i] = 1;   /* first row: any of 64 states */

    for (idx_t r = 1; r < n + 1; ++r) {      /* we need n additional rows to reach height n+1 */
        for (int to = 0; to < 64; ++to) {
            seq_t sum = 0;
            for (int from = 0; from < 64; ++from) {
                if (allowed[from][to])
                    sum += dp_cur[from];
            }
            dp_nxt[to] = sum;
        }
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_nxt[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += dp_cur[i];
    return total;
}
