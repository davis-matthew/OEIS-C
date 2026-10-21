/*
 * A207691  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A207691: Number of n×6 binary arrays avoiding patterns 
 * 0 0 0 and 0 1 0 horizontally, and 0 0 1 and 1 1 1 vertically.
 *
 * The computation is performed by dynamic programming on the last two rows.
 * A row pattern is an allowed 6‑bit string that contains no forbidden
 * horizontal triple.  The number of such patterns is 25, which yields a(1)=25
 * and a(2)=25²=625.  For n≥3 we keep a table dp[i][j] meaning the number
 * of partial arrays ending with the last two rows having indices i and j
 * in the allowed‑rows list.  Transitions are precomputed: for every pair
 * (i,j) we list all k such that the triple of rows (i,j,k) satisfies the
 * vertical constraints.  The algorithm runs in O(n·R²·R) time with R=25,
 * easily below the time limit for n≈10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* allowed row patterns; initialized once */
static int  allowed_rows[64];
static size_t R;            /* number of allowed rows */

/* precomputed transitions: for each (i,j) list of k indices */
static uint8_t  next_count[64][64];
static int      next_idx[64][64][64];   /* up to 64 next states per pair */

/* DP buffers */
static seq_t dp_buf1[64 * 64];
static seq_t dp_buf2[64 * 64];

/* Helper: test whether a 6‑bit row contains an illegal horizontal pattern */
static int row_ok(int row)
{
    /* check 4 possible triples in the 6‑bit pattern */
    for (int col = 0; col < 4; ++col) {
        int b0 = (row >> col)     & 1;
        int b1 = (row >> (col+1)) & 1;
        int b2 = (row >> (col+2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 0) ||
            (b0 == 0 && b1 == 1 && b2 == 0))
            return 0;
    }
    return 1;
}

/* Helper: precompute allowed rows and transitions */
static void init(void)
{
    /* identify allowed rows */
    R = 0;
    for (int row = 0; row < 64; ++row) {
        if (row_ok(row))
            allowed_rows[R++] = row;
    }

    /* precompute transitions */
    for (int i = 0; i < (int)R; ++i) {
        for (int j = 0; j < (int)R; ++j) {
            unsigned cnt = 0;
            for (int k = 0; k < (int)R; ++k) {
                int r1 = allowed_rows[i];
                int r2 = allowed_rows[j];
                int r3 = allowed_rows[k];
                int bad = 0;
                for (int col = 0; col < 6; ++col) {
                    int b1 = (r1 >> col) & 1;
                    int b2 = (r2 >> col) & 1;
                    int b3 = (r3 >> col) & 1;
                    if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                        (b1 == 1 && b2 == 1 && b3 == 1)) {
                        bad = 1; break;
                    }
                }
                if (!bad) {
                    next_idx[i][j][cnt++] = k;
                }
            }
            next_count[i][j] = cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* ensure precomputation has happened */
    static int initialized = 0;
    if (!initialized)
        init(), initialized = 1;

    if (n == 0)
        return (seq_t)0;          /* not part of the sequence, safe to return 0 */

    if (n == 1)
        return (seq_t)R;          /* single row, any allowed pattern */

    if (n == 2) {
        return (seq_t)R * (seq_t)R;  /* any two allowed rows together */
    }

    /* DP for n >= 3 */
    seq_t *dp_cur  = dp_buf1;
    seq_t *dp_nxt  = dp_buf2;
    size_t stride = R;          /* index j in dp as i*stride + j */

    /* init for length 2 */
    memset(dp_cur, 0, stride * stride * sizeof(seq_t));
    for (int i = 0; i < (int)R; ++i)
        for (int j = 0; j < (int)R; ++j)
            dp_cur[i*stride + j] = (seq_t)1;

    /* iterate rows from 3 to n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(dp_nxt, 0, stride * stride * sizeof(seq_t));

        for (int i = 0; i < (int)R; ++i) {
            size_t base_ij = i * stride;
            for (int j = 0; j < (int)R; ++j) {
                seq_t val = dp_cur[base_ij + j];
                if (!val) continue;

                unsigned cnt = next_count[i][j];
                for (unsigned t = 0; t < cnt; ++t) {
                    int k = next_idx[i][j][t];
                    dp_nxt[j*stride + k] += val;
                }
            }
        }
        /* swap dp_cur and dp_nxt */
        seq_t *tmp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp;
    }

    /* sum all entries of final DP table */
    seq_t result = 0;
    for (size_t idx = 0; idx < stride * stride; ++idx)
        result += dp_cur[idx];

    return result;
}
