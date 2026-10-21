/*
 * A205267  Number of (n+2) X 4 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally, and new values 0..1 introduced in row major order.
 * https://oeis.org/A205267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary arrays such that every 3×3
 *            subblock has a horizontal, vertical or NW→SE diagonal of three
 *            equal cells, and the first cell (top‑left) is 0. Implementation
 *            uses a dynamic‑programming over pairs of consecutive rows. Only
 *            the first bit of the first row is forced to 0....
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/*--------------------------------------------------------------------
   transition tables:  for a pair (a,b) (each 4 bits) list of possible
   next pairs (b,c) that keep every 3×3 subblock valid.
--------------------------------------------------------------------*/
static uint8_t  next_cand[256][16];
static uint8_t  next_cnt[256];
static bool     initialized = false;

/*  check whether a 3×3 block formed by rows a,b,c satisfies the
    requirement that it contains at least one line of three equal
    cells (horizontal, vertical or main diagonal) ------------------*/
static bool triple_valid(int a, int b, int c)
{
    for (int j = 0; j < 2; ++j) {                    /* subblock columns j..j+2 */
        bool ok = false;
        /* horizontal lines inside the subblock */
        for (int r = 0; r < 3; ++r) {
            int row = (r==0? a : (r==1? b : c));
            int bits = (row >> j) & 0x7;              /* 3 bits of the row */
            if (bits==0 || bits==7) { ok = true; break; }
        }
        /* vertical lines inside the subblock */
        if (!ok) {
            for (int k = 0; k < 3; ++k) {
                int v0 = (a >> (j+k)) & 1;
                int v1 = (b >> (j+k)) & 1;
                int v2 = (c >> (j+k)) & 1;
                if (v0==v1 && v1==v2) { ok = true; break; }
            }
        }
        /* NW→SE diagonal inside the subblock */
        if (!ok) {
            int d0 = (a >> j)   & 1;
            int d1 = (b >> (j+1)) & 1;
            int d2 = (c >> (j+2)) & 1;
            if (d0==d1 && d1==d2) ok = true;
        }
        if (!ok) return false;
    }
    return true;
}

/*  precompute transitions for all 256 pairs (a,b) ------------------*/
static void init_transitions(void)
{
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            int start = a*16 + b;
            next_cnt[start] = 0;
            for (int c = 0; c < 16; ++c) if (triple_valid(a,b,c)) {
                int next = b*16 + c;
                next_cand[start][next_cnt[start]++] = (uint8_t)next;
            }
        }
    }
    initialized = true;
}

/*--------------------------------------------------------------------
   Public function required by the build framework
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */
    if (!initialized) init_transitions();

    const idx_t rows = n + 2;              /* total number of rows */
    if (rows <= 2) return (seq_t)0;        /* n>=1 ensures rows>=3 */

    /* dp_cur[state] : number of ways to reach the pair (prev,cur)
                       after processing some rows                */
    seq_t dp_cur[256];
    seq_t dp_next[256];
    for (int i = 0; i < 256; ++i)  dp_cur[i] = (seq_t)0;
    /* initialise with all first two rows, but force the top‑left bit of
       the first row to be 0                                        */
    for (int a = 0; a < 16; ++a) if ((a & 1) == 0) {
        for (int b = 0; b < 16; ++b) {
            int idx = a*16 + b;
            dp_cur[idx] = (seq_t)1;
        }
    }

    /* iterate over additional rows beyond the first two */
    idx_t steps = rows - 2;                /* number of transitions */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;
        for (int p = 0; p < 256; ++p) {
            seq_t cur = dp_cur[p];
            if (cur == (seq_t)0) continue;
            uint8_t cnt = next_cnt[p];
            for (uint8_t k = 0; k < cnt; ++k) {
                int q = next_cand[p][k];
                dp_next[q] += cur;
            }
        }
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all ways; each corresponds to a distinct array */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 256; ++i) ans += dp_cur[i];
    return ans;
}
