/*
 * A237489  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A237489
   (Number of (n+1)×2 arrays over {0,1,2,3} whose 2×2 block values
    differ by exactly one from their vertical neighbors)

   The algorithm counts arrays by dynamic programming over states
   (previous row, block value).  For each adjacent pair of rows we
   compute the 2×2 block value
          V = max + upper_median – lower_median – min
   and enforce |V_next – V_prev| = 1 for every consecutive block.
   The DP runs in O(n) time with 112 states (16 rows × 7 possible
   block values) and uses only integer arithmetic.  The implementation
   works for any unsigned integer type seq_t and for the arbitrary
   precision implementation provided by the build.  No external
   libraries are required. */
 
#include <stdint.h>

#define ROWS   16   /* number of 2‑column rows (0..3 each)          */
#define BLOCKS 7    /* possible values of the block (0..6)         */
#define STATES (ROWS*BLOCKS)

static int block_table[ROWS][ROWS] = {0};
static int block_table_init = 0;

/* Compute the 2×2 block value from two rows r0 and r1 (each 0..15) */
static inline int compute_block(int r0, int r1)
{
    if (!block_table_init) {
        /* initialise the table once */
        for (int a = 0; a < ROWS; ++a) {
            int a0 = a >> 2;           /* high digit (0..3)      */
            int a1 = a & 3;            /* low  digit (0..3)      */
            for (int b = 0; b < ROWS; ++b) {
                int b0 = b >> 2;
                int b1 = b & 3;
                int v[4] = {a0, a1, b0, b1};
                /* simple insertion sort for 4 elements */
                for (int i = 1; i < 4; ++i) {
                    int key = v[i];
                    int j = i - 1;
                    while (j >= 0 && v[j] > key) {
                        v[j + 1] = v[j];
                        --j;
                    }
                    v[j + 1] = key;
                }
                block_table[a][b] = v[3] + v[2] - v[1] - v[0];
            }
        }
        block_table_init = 1;
    }
    return block_table[r0][r1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    /* initialise current state counts for n = 1 (first block) */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int blk = compute_block(r0, r1);
            int state = r1 * BLOCKS + blk;  /* state = (previous row, block value) */
            cur[state] += 1;
        }
    }
    /* process remaining n-1 blocks */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = cur[s];
            if (!cnt) continue;
            int prev_row = s / BLOCKS;
            int prev_blk = s % BLOCKS;
            for (int next_row = 0; next_row < ROWS; ++next_row) {
                int next_blk = compute_block(prev_row, next_row);
                if (next_blk == prev_blk + 1 || next_blk + 1 == prev_blk) {
                    int ns = next_row * BLOCKS + next_blk;
                    nxt[ns] += cnt;
                }
            }
        }
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }
    /* sum all ending states */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += cur[i];
    return total;
}
