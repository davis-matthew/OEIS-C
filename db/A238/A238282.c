/*
 * A238282  Number of (n+1)X(2+1) 0..3 arrays with no element greater than all
 *          horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A238282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A238282: Number of (n+1)×3 arrays of 0..3 with no element greater than all horizontal
 * neighbors and no element less than all vertical neighbors.
 * Computed by dynamic programming on pairs of consecutive rows.
 */

#include <stdint.h>
#include <stdbool.h>

/* Local tables */
static int   row_idx[64] = {0};          /* mapping from 6‑bit code to index –1 if invalid   */
static int   row_cfg[16];                /* original 6‑bit code of each valid row          */
static int   row_val[16][3];             /* decoded 3 values of each valid row             */
static uint16_t row_ge_mask[16];         /* bit j set if row i >= row j (element-wise)     */
static uint16_t vert_mask[16][16];       /* mask of next rows k for state (i,j)             */
static bool   tables_initialized = false;
static const uint32_t TOT_ROWS = 16;     /* number of valid row configurations              */

/* Build all tables once */
static void init_tables(void)
{
    if (tables_initialized) return;
    int next = 0;
    for (int code = 0; code < 64; ++code) {
        int d[3] = {code & 3, (code >> 2) & 3, (code >> 4) & 3};
        bool ok = true;
        /* horizontal constraints */
        if (!(d[0] <= d[1] && d[2] <= d[1])) ok = false;
        if (!((d[1] <= d[0]) || (d[1] <= d[2]))) ok = false;
        if (ok) {
            row_idx[code] = next;
            row_cfg[next] = code;
            for (int c = 0; c < 3; ++c) row_val[next][c] = d[c];
            ++next;
        } else {
            row_idx[code] = -1;
        }
    }

    /* build row_ge_mask */
    for (int i = 0; i < TOT_ROWS; ++i) {
        uint16_t m = 0;
        for (int j = 0; j < TOT_ROWS; ++j) {
            bool ge = true;
            for (int c = 0; c < 3; ++c)
                if (row_val[i][c] < row_val[j][c]) { ge = false; break; }
            if (ge) m |= (1u << j);
        }
        row_ge_mask[i] = m;
    }

    /* build vertical compatibility mask: for each pair (i,j) (prev, curr)
       enumerate next rows k such that curr is not less than both neighbors */
    for (int i = 0; i < TOT_ROWS; ++i) {          /* i = previous row */
        for (int j = 0; j < TOT_ROWS; ++j) {      /* j = current row */
            uint16_t m = 0;
            for (int k = 0; k < TOT_ROWS; ++k) {  /* k = next row      */
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    if (row_val[j][c] < row_val[i][c] && row_val[j][c] < row_val[k][c]) {
                        ok = false; break;
                    }
                }
                if (ok) m |= (1u << k);
            }
            vert_mask[i][j] = m;
        }
    }

    tables_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays for two consecutive rows: state id = prev*16 + curr  (0..255) */
    seq_t dp[256] = {0};
    seq_t nextdp[256] = {0};

    /* initialization: first two rows (top and middle).  Top >= middle */
    for (int i = 0; i < TOT_ROWS; ++i) {
        int ge = row_ge_mask[i];
        for (int j = 0; j < TOT_ROWS; ++j)
            if (ge & (1u << j)) dp[(i << 4) | j] = 1;
    }

    idx_t steps = n - 1;   /* number of additional rows after the first two */
    for (idx_t s = 0; s < steps; ++s) {
        /* reset nextdp */
        for (int i = 0; i < 256; ++i) nextdp[i] = 0;

        /* transition */
        for (int id = 0; id < 256; ++id) {
            seq_t val = dp[id];
            if (!val) continue;
            int prev   = id >> 4;
            int curr   = id & 15;
            uint16_t m = vert_mask[prev][curr];
            for (int nxt = 0; nxt < TOT_ROWS; ++nxt) {
                if (m & (1u << nxt)) {
                    int nid = (curr << 4) | nxt;
                    nextdp[nid] += val;
                }
            }
        }

        /* swap dp */
        for (int i = 0; i < 256; ++i) dp[i] = nextdp[i];
    }

    /* final bottom-row constraint: bottom >= above */
    seq_t result = 0;
    for (int id = 0; id < 256; ++id) {
        seq_t val = dp[id];
        if (!val) continue;
        int prev = id >> 4;
        int curr = id & 15;
        if (row_ge_mask[curr] & (1u << prev))
            result += val;
    }
    return result;
}
