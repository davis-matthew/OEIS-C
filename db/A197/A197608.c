/*
 * A197608  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,2,1,1,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over 3‑column rows. Each
 *            cell value x (0..4) must have exactly required[x] horizontal or
 *            vertical neighbors equal to target[x], where target[0] = 4,
 *            required[0] = 0 target[1] = 2, required[1] = 1 target[2] = 1,
 *            required[2] = 2 target[3] = 1, required[3] = 3...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Number of possible 3‑digit rows (0..4) */
#define ROWS 125

/* decode a row index (0..124) into its three digits 0..4 */
static inline void decode_row(int idx, int out[3])
{
    int t = idx;
    out[2] = t % 5; t /= 5;
    out[1] = t % 5; t /= 5;
    out[0] = t % 5;
}

/* neighbor rule constants */
static const int target_val[5] = {4, 2, 1, 1, 1};
static const int required_cnt[5] = {0, 1, 2, 3, 4};

/* check whether a row 'cur' satisfies the rule when its vertical
   neighbors are 'above' and 'below'.  If a neighbor does not exist,
   hasAbove/hasBelow are false and its digit array is left unchanged. */
static inline bool check_row(int cur, int above, int below,
                             bool hasAbove, bool hasBelow)
{
    int curv[3], av[3] = {0,0,0}, bv[3] = {0,0,0};

    decode_row(cur, curv);
    if (hasAbove) decode_row(above, av);
    if (hasBelow) decode_row(below, bv);

    for (int c = 0; c < 3; ++c) {
        int val = curv[c];
        int cnt = 0;
        int t = target_val[val];

        /* horizontal neighbours */
        if (c > 0   && curv[c-1] == t)  ++cnt;
        if (c < 2   && curv[c+1] == t)  ++cnt;
        /* vertical neighbours */
        if (hasAbove && av[c] == t)     ++cnt;
        if (hasBelow && bv[c] == t)     ++cnt;

        if (cnt != required_cnt[val])   return false;
    }
    return true;
}

/* transition tables */
static unsigned char  *next_rows = NULL; /* flattened successor lists */
static unsigned char  *pair_len  = NULL; /* number of successors per (prev,curr) */
static uint32_t       *pair_off  = NULL; /* offset into next_rows */

/* cache of decoded row digits */
static int rowVals[ROWS][3];

/* validity tables */
static unsigned char validFirst[ROWS][ROWS];
static unsigned char validLast [ROWS][ROWS];
static unsigned char validSolo  [ROWS];

/* DP buffers */
static seq_t dp0[ROWS][ROWS];
static seq_t dp1[ROWS][ROWS];

/* initialization flag */
static bool tables_ready = false;

/* build transition tables, validity tables and row digit cache */
static void init_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    /* decode all rows once */
    for (int i = 0; i < ROWS; ++i)
        decode_row(i, rowVals[i]);

    /* compute validity of first and last row pairs */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            validFirst[i][j] = check_row(i, -1, j, false, true)  ? 1 : 0;
            validLast [i][j] = check_row(j, i, -1, true,  false) ? 1 : 0;
        }
        validSolo[i] = check_row(i, -1, -1, false, false) ? 1 : 0;
    }

    /* transition lists for each (prev,curr) pair */
    int pairSize = ROWS * ROWS;
    pair_len = (unsigned char*)malloc(pairSize * sizeof(unsigned char));
    pair_off = (uint32_t*)malloc(pairSize * sizeof(uint32_t));

    for (int p = 0; p < ROWS; ++p)
        for (int c = 0; c < ROWS; ++c) {
            unsigned char cnt = 0;
            for (int n = 0; n < ROWS; ++n)
                if (check_row(c, p, n, true, true))
                    ++cnt;
            pair_len[p * ROWS + c] = cnt;
        }

    /* compute offsets and total list size */
    uint32_t sum = 0;
    for (int i = 0; i < pairSize; ++i) {
        pair_off[i] = sum;
        sum += pair_len[i];
    }

    next_rows = (unsigned char*)malloc(sum * sizeof(unsigned char));

    /* fill successor lists */
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            uint32_t off = pair_off[p * ROWS + c];
            uint32_t pos = off;
            for (int n = 0; n < ROWS; ++n)
                if (check_row(c, p, n, true, true))
                    next_rows[pos++] = (unsigned char)n;
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_tables();

    /* clear DP buffers */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp0[i][j] = dp1[i][j] = 0;

    seq_t (*curr)[ROWS] = dp0;
    seq_t (*next)[ROWS] = dp1;

    /* initialise with valid first two rows */
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1)
            if (validFirst[r0][r1])
                curr[r0][r1] = (seq_t)1;

    /* special case n==1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < ROWS; ++r)
            if (validSolo[r]) ans += (seq_t)1;
        return ans;
    }

    /* traversal of intermediate rows */
    for (int step = 2; step <= n-1; ++step) {
        /* zero next buffer */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                next[i][j] = 0;

        /* transition */
        for (int pPrev = 0; pPrev < ROWS; ++pPrev) {
            for (int pCurr = 0; pCurr < ROWS; ++pCurr) {
                seq_t val = curr[pPrev][pCurr];
                if (val == 0) continue;

                uint32_t off = pair_off[pPrev * ROWS + pCurr];
                unsigned char len = pair_len[pPrev * ROWS + pCurr];

                for (unsigned char k = 0; k < len; ++k) {
                    unsigned char nRow = next_rows[off + k];
                    next[pCurr][nRow] += val;
                }
            }
        }
        /* swap buffers */
        seq_t (*tmp)[ROWS] = curr;
        curr = next;
        next = tmp;
    }

    /* sum over valid last row */
    seq_t ans = 0;
    for (int p = 0; p < ROWS; ++p)
        for (int c = 0; c < ROWS; ++c)
            if (validLast[p][c])
                ans += curr[p][c];

    return ans;
}
