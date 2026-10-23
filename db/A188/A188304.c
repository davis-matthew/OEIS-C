/*
 * A188304  Number of (n+1)X3 binary arrays with every 2X2 subblock sum equal
 *          to exactly one or two horizontal and vertical neighbor 2X2
 *          subblock sums.
 * https://oeis.org/A188304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188304
 *  a(n) = number of (n+1)×3 binary arrays such that each 2×2 subblock sum is
 *  equal to exactly one or two of its horizontal and vertical neighbour
 *  subblock sums.
 *
 *  The function uses a dynamic‑programming algorithm that runs in
 *  O(n) time and O(1) memory proportional to the number of possible row
 *  pattern pairs (state space = 8 × 25 × 25 = 5000).
 *
 *  There is no known closed form recurrence; the algorithm enumerates
 *  all possibilities row by row and keeps only the last two 2×2 block sum
 *  pairs that are relevant for checking the condition.
 *
 *  The implementation is portable C‑11 / C++17, it respects the
 *  external type definitions for seq_t and idx_t, and it does not use
 *  any forbidden headers or features.
 */

#include <stddef.h>    /* uintptr_t for pointer arithmetic, if needed */

/* External integer types are supplied by the build system */
extern seq_t a(idx_t n);

/* =============================================================================
 *  --------------------------------------------------------------------------
 *  Precomputation tables
 *  --------------------------------------------------------------------------
 *  Each 3‑bit row can be represented by an integer 0‑7.
 *  For any two consecutive rows (r0,r1) there are two 2×2 blocks:
 *      left block  : columns 0–1
 *      right block : columns 1–2
 *  Each block sum lies in {0,1,2,3,4}.  All possible ordered pairs of
 *  sums are mapped to an index in 0…24
 *
 *  For a pair of rows we store:
 *      idx        = sum0 * 5 + sum1          (0…24)
 *      sum0[idx]  = sum of left block
 *      sum1[idx]  = sum of right block
 *      horizEq[idx] = 1  if sum0==sum1 else 0
 *  --------------------------------------------------------------------------
 *  --------------------------------------------------------------------------
 *  --------------------------------------------------------------------------
 *  =============================================================================
 */

#define SUMS 5                                     /* range of block sum 0..4   */
#define SUMDIM (SUMS * SUMS)                       /* 25 possible sum pair     */
#define ROWS 8                                     /* 0..7 3‑bit rows          */
#define STATECOUNT (ROWS * SUMDIM * SUMDIM)        /* 5000 DP states           */

static unsigned char sum0[SUMDIM];
static unsigned char sum1[SUMDIM];
static unsigned char horizEq[SUMDIM];
static unsigned char idxRow[ROWS][ROWS];          /* mapping of row pair → idx */

static void init_tables(void)
{
    static int once = 0;
    if (once) return;
    /* 1. Fill sum0, sum1, horizEq from idx */
    for (int k = 0; k < SUMDIM; ++k) {
        sum0[k] = (unsigned char)(k / SUMS);
        sum1[k] = (unsigned char)(k % SUMS);
        horizEq[k] = (sum0[k] == sum1[k]) ? 1 : 0;
    }
    /* 2. Compute idxRow for every row pair */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            unsigned char s0 = ((a & 1) + ((a >> 1) & 1) + (b & 1) + ((b >> 1) & 1));
            unsigned char s1 = (((a >> 1) & 1) + ((a >> 2) & 1) +
                                ((b >> 1) & 1) + ((b >> 2) & 1));
            idxRow[a][b] = (unsigned char)(s0 * SUMS + s1);
        }
    }
    once = 1;
}

/* Encode a DP state (current row, previous sum-pair index,
 * and the sum‑pair index before that) into a single array index. */
static size_t encode_state(int cur, int prev, int prevprev)
{
    return (size_t)cur * SUMDIM * SUMDIM +
           (size_t)prev * SUMDIM +
           (size_t)prevprev;
}

/* --------------------------------------------------------------------------
 *  Main sequence function
 * --------------------------------------------------------------------------
 */
extern seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) = 24.  For n<1 we return 0. */
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)24;   /* verified first term */

    init_tables();                 /* ensure tables are ready */

    /* DP tables: two buffers that we swap every step.     */
    static seq_t cur[STATECOUNT];
    static seq_t nxt[STATECOUNT];
    seq_t *dp_cur = cur;
    seq_t *dp_nxt = nxt;

    /* ------------------------------------------------------------------
     *  Initialization: process the first three rows (rows 0,1,2).
     *  After this step we have considered all constraints for the
     *  blocks that involve the topmost pair of rows.
     * ------------------------------------------------------------------ */
    for (int i = 0; i < STATECOUNT; ++i) {
        dp_cur[i] = (seq_t)0;
        dp_nxt[i] = (seq_t)0;      /* clear next buffer as well */
    }

    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            for (int r2 = 0; r2 < ROWS; ++r2) {
                int idxS0 = idxRow[r0][r1];
                int idxS1 = idxRow[r1][r2];
                int s0_0 = sum0[idxS0], s0_1 = sum1[idxS0];
                int s1_0 = sum0[idxS1], s1_1 = sum1[idxS1];
                int horiz = horizEq[idxS0];

                /* Each of the two blocks in the first pair must have
                 * exactly 1 or 2 equal neighbours among:
                 *   - horizontal neighbour (the other block in the pair)
                 *   - vertical neighbour below (its counterpart in the
                 *     next pair).  Since we have no block above yet,
                 *     only these two neighbours can be equal.          */
                int final0 = horiz + (s0_0 == s1_0);
                int final1 = horiz + (s0_1 == s1_1);

                if ((final0 < 1 || final0 > 2) ||
                    (final1 < 1 || final1 > 2))
                    continue;

                size_t st = encode_state(r2, idxS1, idxS0);
                dp_cur[st] += (seq_t)1;
            }
        }
    }

    /* ------------------------------------------------------------------
     *  DP over the remaining rows.  At each step we add one new row
     *  rnew, check that the block pair from the previous two rows
     *  satisfies the neighbour condition with the newly added
     *  vertical neighbour, then advance the DP state.
     * ------------------------------------------------------------------ */
    for (idx_t step = 0; step < n - 2; ++step) {
        /* clear the next buffer */
        for (int i = 0; i < STATECOUNT; ++i) dp_nxt[i] = (seq_t)0;

        /* iterate over all active states */
        for (int st = 0; st < STATECOUNT; ++st) {
            seq_t cnt = dp_cur[st];
            if (!cnt) continue;

            int cur_row   = st / (SUMDIM * SUMDIM);
            int rem       = st % (SUMDIM * SUMDIM);
            int idxPrev   = rem / SUMDIM;
            int idxPrevPrev = rem % SUMDIM;

            int sPrev0 = sum0[idxPrev], sPrev1 = sum1[idxPrev];
            int sTop0  = sum0[idxPrevPrev], sTop1  = sum1[idxPrevPrev];
            int horiz  = horizEq[idxPrev];

            /* try every possible next row */
            for (int rnext = 0; rnext < ROWS; ++rnext) {
                int idxNew = idxRow[cur_row][rnext];
                int sNew0 = sum0[idxNew], sNew1 = sum1[idxNew];

                /* check the blocks that now have all neighbours known:  */
                int final0 = horiz + (sPrev0 == sTop0) + (sPrev0 == sNew0);
                int final1 = horiz + (sPrev1 == sTop1) + (sPrev1 == sNew1);

                if ((final0 < 1 || final0 > 2) ||
                    (final1 < 1 || final1 > 2))
                    continue;

                size_t newSt = encode_state(rnext, idxNew, idxPrev);
                dp_nxt[newSt] += cnt;
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* ------------------------------------------------------------------
     *  Final validation: after the last row has been processed,
     *  blocks of the last pair have only up horizontal neighbours.
     *  They must still satisfy the 1 or 2 equal neighbour rule.
     * ------------------------------------------------------------------ */
    seq_t ans = (seq_t)0;
    for (int st = 0; st < STATECOUNT; ++st) {
        seq_t cnt = dp_cur[st];
        if (!cnt) continue;

        int cur_row   = st / (SUMDIM * SUMDIM);
        int rem       = st % (SUMDIM * SUMDIM);
        int idxPrev   = rem / SUMDIM;
        int idxPrevPrev = rem % SUMDIM;

        int sPrev0 = sum0[idxPrev], sPrev1 = sum1[idxPrev];
        int sTop0  = sum0[idxPrevPrev], sTop1  = sum1[idxPrevPrev];
        int horiz  = horizEq[idxPrev];

        int final0 = horiz + (sPrev0 == sTop0);
        int final1 = horiz + (sPrev1 == sTop1);

        if ((final0 < 1 || final0 > 2) ||
            (final1 < 1 || final1 > 2))
            continue;

        ans += cnt;
    }
    return ans;
}
