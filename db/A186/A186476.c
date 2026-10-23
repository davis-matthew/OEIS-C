/*
 * A186476  Number of (n+1)X4 0..3 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-4)-3*a(n-5) for n>14
 *            Initial values a(1)..a(14) are computed by a dynamic‑programming
 *            algorithm that explores all (n+1)×4 arrays satisfying the
 *            commuting constraints
 * Range:     u8 none, u16 n <= 6, u32 n <= 22, u64 n <= 52, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* memset */

seq_t a(idx_t n)
{
    /* ---------------  static data used for all calls  --------------- */
    /* 128‑bit integer temporary macro for small constants */
    #define S(x) ((seq_t)(x))

    /* block digits: block id -> 4 digits (top-left, top-right,
       bottom-left, bottom-right) */
    static uint8_t block_dig[256][4];
    /* block id for two rows and a column start (0,1,2) */
    static uint8_t block_id[256][256][3];
    /* matrix commutation table */
    static unsigned char commute_tbl[256][256];
    /* all valid row pairs (horizontal constraints only) */
    static int valid_r1[2000];  /* enough capacity, real count < 1100 */
    static int valid_r2[2000];
    static int  idx_map[256][256];   /* -1 means invalid pair */
    static int  valid_cnt = 0;
    /* DP arrays for counting */
    static seq_t *dp_curr = NULL;
    static seq_t *dp_next = NULL;
    /* store initial 14 values (index 1..14) */
    static seq_t init_val[15];
    static int init_done = 0;

    /* --------------------------------------------------------------- */
    if (!init_done) {
        /* fill idx_map with -1 */
        memset(idx_map, 0xFF, sizeof(idx_map));

        /* 1. decode digits for each 4‑digit block (0..255) */
        for (int id = 0; id < 256; ++id) {
            block_dig[id][0] = (id >> 6) & 3;
            block_dig[id][1] = (id >> 4) & 3;
            block_dig[id][2] = (id >> 2) & 3;
            block_dig[id][3] =  id       & 3;
        }

        /* 2. compute block_id table for all pairs of rows and col starts */
        for (int r1 = 0; r1 < 256; ++r1) {
            for (int r2 = 0; r2 < 256; ++r2) {
                for (int c = 0; c < 3; ++c) {
                    uint8_t a = block_dig[r1][c];
                    uint8_t b = block_dig[r1][c+1];
                    uint8_t c1= block_dig[r2][c];
                    uint8_t d = block_dig[r2][c+1];
                    block_id[r1][r2][c] = (a << 6) | (b << 4) | (c1 << 2) | d;
                }
            }
        }

        /* 3. compute commutation table */
        for (int A = 0; A < 256; ++A) {
            uint8_t a0 = block_dig[A][0], a1 = block_dig[A][1],
                    a2 = block_dig[A][2], a3 = block_dig[A][3];
            for (int B = 0; B < 256; ++B) {
                uint8_t b0 = block_dig[B][0], b1 = block_dig[B][1],
                        b2 = block_dig[B][2], b3 = block_dig[B][3];
                /* AB */
                int r00 = a0*b0 + a1*b2;
                int r01 = a0*b1 + a1*b3;
                int r10 = a2*b0 + a3*b2;
                int r11 = a2*b1 + a3*b3;
                /* BA */
                int s00 = b0*a0 + b1*a2;
                int s01 = b0*a1 + b1*a3;
                int s10 = b2*a0 + b3*a2;
                int s11 = b2*a1 + b3*a3;
                commute_tbl[A][B] = (r00==s00 && r01==s01 && r10==s10 && r11==s11);
            }
        }

        /* 4. find all valid row pairs (horizontal constraints only) */
        for (int r1 = 0; r1 < 256; ++r1) {
            for (int r2 = 0; r2 < 256; ++r2) {
                uint8_t b0 = block_id[r1][r2][0];
                uint8_t b1 = block_id[r1][r2][1];
                uint8_t b2 = block_id[r1][r2][2];
                if (commute_tbl[b0][b1] && commute_tbl[b1][b2]) {
                    idx_map[r1][r2] = valid_cnt;
                    valid_r1[valid_cnt] = r1;
                    valid_r2[valid_cnt] = r2;
                    ++valid_cnt;
                }
            }
        }
        /* 5. allocate DP arrays */
        dp_curr = (seq_t*)malloc(valid_cnt * sizeof(seq_t));
        dp_next = (seq_t*)malloc(valid_cnt * sizeof(seq_t));
        if (!dp_curr || !dp_next) {
            /* allocation failure – fall back to zero */
            return 0;
        }

        /* 6. initialise DP for n = 1 (two rows) */
        for (int i = 0; i < valid_cnt; ++i)
            dp_curr[i] = S(1);
        seq_t total = S(0);
        for (int i = 0; i < valid_cnt; ++i)
            total = total + dp_curr[i];
        init_val[1] = total;

        /* 7. iteratively build arrays up to n = 14 */
        for (int rows = 2; rows <= 14; ++rows) {
            /* clear next DP */
            memset(dp_next, 0, valid_cnt * sizeof(seq_t));
            for (int idx = 0; idx < valid_cnt; ++idx) {
                seq_t cnt = dp_curr[idx];
                if (cnt == S(0)) continue;
                int r_pp = valid_r1[idx];
                int r_p  = valid_r2[idx];
                for (int r_n = 0; r_n < 256; ++r_n) {
                    int nxt_idx = idx_map[r_p][r_n];
                    if (nxt_idx < 0) continue;
                    /* vertical commutation checks for all three col pairs */
                    int ok = 1;
                    for (int c = 0; c < 3; ++c) {
                        if (!commute_tbl[ block_id[r_pp][r_p][c] ]
                              [ block_id[r_p][r_n][c] ] ) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        dp_next[nxt_idx] = dp_next[nxt_idx] + cnt;
                    }
                }
            }
            /* compute total for this array size */
            total = S(0);
            for (int i = 0; i < valid_cnt; ++i)
                total = total + dp_next[i];
            init_val[rows] = total;
            /* swap current and next DP */
            seq_t *tmp = dp_curr;
            dp_curr = dp_next;
            dp_next = tmp;
        }
        init_done = 1;
    }
    /* --------------------------------------------------------------- */

    if (n <= 14)
        return init_val[n];

    /* --- use recurrence for all larger n --- */
    seq_t buf[5];
    buf[0] = init_val[10];
    buf[1] = init_val[11];
    buf[2] = init_val[12];
    buf[3] = init_val[13];
    buf[4] = init_val[14];
    seq_t result = S(0);

    for (idx_t k = 15; k <= n; ++k) {
        seq_t pos = buf[4]*S(2) + buf[3]*S(2) + buf[1]*S(6);
        seq_t neg = buf[2]*S(6) + buf[0]*S(3);
        seq_t next = pos - neg;

        /* shift buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;

        if (k == n)
            result = next;
    }

    return result;
}
