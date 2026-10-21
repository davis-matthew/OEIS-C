/*
 * A204374  Number of (n+2)X3 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 39, u64 n <= 84, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP: Count (n+2)×3 binary arrays with first cell = 0 such that every 3×3 subblock
   has exactly three equal lines (rows, columns or the NW→SE diagonal).
   This implements the definition of A204374 without using the empirical recurrence. */
#include <stdbool.h>

static bool init = false;
static bool valid[8][8][8];

/* Test whether a 3×3 block defined by rows a,b,c has exactly three equal lines. */
static inline bool triple_valid(uint8_t a, uint8_t b, uint8_t c)
{
    int count = 0;
    int a0 = (a >> 0) & 1, a1 = (a >> 1) & 1, a2 = (a >> 2) & 1;
    int b0 = (b >> 0) & 1, b1 = (b >> 1) & 1, b2 = (b >> 2) & 1;
    int c0 = (c >> 0) & 1, c1 = (c >> 1) & 1, c2 = (c >> 2) & 1;

    if (a0 == a1 && a1 == a2) ++count;          /* row a */
    if (b0 == b1 && b1 == b2) ++count;          /* row b */
    if (c0 == c1 && c1 == c2) ++count;          /* row c */
    if (a0 == b0 && b0 == c0) ++count;          /* column 0 */
    if (a1 == b1 && b1 == c1) ++count;          /* column 1 */
    if (a2 == b2 && b2 == c2) ++count;          /* column 2 */
    if (a0 == b1 && b1 == c2) ++count;          /* NW–SE diagonal */

    return count == 3;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0 per offset 1 */

    /* Build the validity table once. */
    if (!init) {
        init = true;
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                for (int c = 0; c < 8; ++c)
                    valid[a][b][c] = triple_valid((uint8_t)a, (uint8_t)b, (uint8_t)c);
    }

    /* cnt[prev<<3 | curr] holds number of ways to reach that pair after a given number
       of added rows.  For the first step the first row must start with 0 (bit 0 of
       prev row == 0) to enforce the “new values 0..1 introduced in row major order”. */
    seq_t cnt[64] = {0}, nxt[64];
    for (int i = 0; i < 64; ++i) {
        if ((i >> 3) & 1) /* bit 0 of the first row ≠ 0 → disallow */
            cnt[i] = 0;
        else
            cnt[i] = 1;
    }

    /* Each iteration adds one new row, so after n steps we have n+2 rows. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) nxt[i] = 0;
        for (int idx = 0; idx < 64; ++idx) {
            seq_t ways = cnt[idx];
            if (!ways) continue;
            int prev = idx >> 3;
            int curr = idx & 7;
            for (int nxtrow = 0; nxtrow < 8; ++nxtrow) {
                if (valid[prev][curr][nxtrow]) {
                    int nidx = (curr << 3) | nxtrow;
                    nxt[nidx] += ways;
                }
            }
        }
        for (int i = 0; i < 64; ++i) cnt[i] = nxt[i];
    }

    /* Sum all final states. */
    seq_t res = 0;
    for (int i = 0; i < 64; ++i) res += cnt[i];
    return res;
}
