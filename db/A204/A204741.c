/*
 * A204741  Number of (n+2)X3 0..2 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally exactly one way, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204741: Number of (n+2)×3 arrays with entries 0..2 such that each 3×3
 * subblock contains exactly one line of three equal elements (horizontal,
 * vertical, diagonal or anti‑diagonal).  Each array is counted once in the
 * canonical form where new symbols 0..2 appear in row‑major order:
 * the first element must be 0 and every time a new value is used it must be
 * one larger than all previously used values.  
 * This implementation uses a transfer‑matrix dynamic‑programming approach
 * that enumerates canonical arrays directly.  The algorithm runs in
 * O(n) time with a small, fixed memory footprint. */

#include <stddef.h>
#include <stdbool.h>

#define ROWS 27                      /* 3^3 possible rows (0..2)**3   */
#define SMODES 3                     /* current maximum label seen (0,1,2) */
#define STATE_COUNT (ROWS * ROWS * SMODES)

/* Global tables built once.  They are zero‑initialized. */
static unsigned char rowval[ROWS][3];
static bool allowed_mask[ROWS][ROWS][ROWS];
static unsigned char trans_cnt[ROWS][ROWS][SMODES];
static unsigned char trans_c[ROWS][ROWS][SMODES][ROWS];
static unsigned char trans_newsm[ROWS][ROWS][SMODES][ROWS];
static bool built_rule = false;

/* Precomputation step ------------------------------------------------------ */
static void build_rule(void)
{
    if (built_rule) return;

    /* decode integer 0..26 into triple of digits 0..2 */
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        rowval[r][0] = tmp / 9;
        rowval[r][1] = (tmp / 3) % 3;
        rowval[r][2] = tmp % 3;
    }

    /* allowed_mask[a][b][c] = true iff the 3×3 block (a,b,c) has exactly
     * one line of equal elements. */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            for (int c = 0; c < ROWS; ++c) {
                int g[3][3];
                g[0][0] = rowval[a][0];
                g[0][1] = rowval[a][1];
                g[0][2] = rowval[a][2];
                g[1][0] = rowval[b][0];
                g[1][1] = rowval[b][1];
                g[1][2] = rowval[b][2];
                g[2][0] = rowval[c][0];
                g[2][1] = rowval[c][1];
                g[2][2] = rowval[c][2];
                int lines = 0;
                for (int r = 0; r < 3; ++r)
                    if (g[r][0] == g[r][1] && g[r][1] == g[r][2]) ++lines;
                for (int r = 0; r < 3; ++r)
                    if (g[0][r] == g[1][r] && g[1][r] == g[2][r]) ++lines;
                if (g[0][0] == g[1][1] && g[1][1] == g[2][2]) ++lines;
                if (g[0][2] == g[1][1] && g[1][1] == g[2][0]) ++lines;
                allowed_mask[a][b][c] = (lines == 1);
            }

    /* precompute all transitions (a,b,sm) -> (b,c,new_sm) that respect
     * both the 3×3 condition and canonical labeling. */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            for (int sm = 0; sm < SMODES; ++sm) {
                unsigned char cnt = 0;
                for (int c = 0; c < ROWS; ++c) {
                    if (!allowed_mask[a][b][c]) continue;

                    /* canonical rule on row c */
                    int cur_sm = sm;
                    bool ok = true;
                    for (int k = 0; k < 3; ++k) {
                        int val = rowval[c][k];
                        if (val <= cur_sm) {
                            /* existing label, allowed */
                        } else if (val == cur_sm + 1 && val <= 2) {
                            cur_sm = val;      /* introduce new symbol */
                        } else {
                            ok = false; break; /* violation */
                        }
                    }
                    if (!ok) continue;

                    trans_c[a][b][sm][cnt] = (unsigned char)c;
                    trans_newsm[a][b][sm][cnt] = (unsigned char)cur_sm;
                    ++cnt;
                }
                trans_cnt[a][b][sm] = cnt;
            }

    built_rule = true;
}

/* Main function required by the sheet ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 */

    build_rule();

    /* state indices: (a,b,sm) -> a*ROWS*SMODES + b*SMODES + sm */
    const size_t steps = (size_t)n;   /* number of rows added after the first two */

    static seq_t curr[STATE_COUNT];
    static seq_t nxt [STATE_COUNT];

    /* clear current state array */
    for (size_t i = 0; i < STATE_COUNT; ++i) curr[i] = 0;

    /* initialise with all legal pairs of first two rows
     * (first element of the very first row must be 0). */
    for (int a = 0; a < ROWS; ++a) {
        if (rowval[a][0] != 0) continue;            /* first cell must be 0 */
        for (int b = 0; b < ROWS; ++b) {
            int sm = 0;
            bool ok = true;
            /* process first row a */
            for (int k = 0; k < 3; ++k) {
                int val = rowval[a][k];
                if (val <= sm) { }
                else if (val == sm + 1 && val <= 2) sm = val;
                else { ok = false; break; }
            }
            /* process second row b */
            if (ok) {
                for (int k = 0; k < 3 && ok; ++k) {
                    int val = rowval[b][k];
                    if (val <= sm) { }
                    else if (val == sm + 1 && val <= 2) sm = val;
                    else { ok = false; break; }
                }
            }
            if (!ok) continue;
            size_t idx = ((size_t)a * ROWS + b) * SMODES + sm;
            curr[idx] += 1;
        }
    }

    /* dynamic programming: add (n) more rows */
    for (size_t step = 0; step < steps; ++step) {
        /* zero next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                for (int sm = 0; sm < SMODES; ++sm) {
                    size_t idx = ((size_t)a * ROWS + b) * SMODES + sm;
                    seq_t v = curr[idx];
                    if (!v) continue;
                    unsigned char cnt = trans_cnt[a][b][sm];
                    for (unsigned char t = 0; t < cnt; ++t) {
                        unsigned char c = trans_c[a][b][sm][t];
                        unsigned char new_sm = trans_newsm[a][b][sm][t];
                        size_t nidx = ((size_t)b * ROWS + c) * SMODES + new_sm;
                        nxt[nidx] += v;
                    }
                }

        /* copy nxt into curr for next iteration */
        for (size_t i = 0; i < STATE_COUNT; ++i) curr[i] = nxt[i];
    }

    /* sum all valid final states */
    seq_t total = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) total += curr[i];
    return total;
}
