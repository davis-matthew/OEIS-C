/*
 * A197059  Number of nX6 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors equal to 2.
 * https://oeis.org/A197059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A197059 – Number of n×6 arrays of 0..4 in which each element
   equals the number of its horizontal and vertical neighbors that are 2.
   The sequence is computed by dynamic programming over binary patterns
   of cells that are equal to 2.  For each n, the DP processes rows
   top‑to‑bottom, keeping a 4096‑state table indexed by (previous row,
   current row) masks.  The number of ways is the sum of the final
   DP‑entries that allow a zero row below the last row. 
   Complexity : O(n·4096·64) time, O(4096) memory. 
   The implementation works for any seq_t/idx_t types provided by the
   build environment. 
*/

#include <stdbool.h>
#include <stdint.h>

#define MASKS 64
#define STATE  (MASKS * MASKS)

/* Transition lists: for each state (U,C) store all admissible
   next row masks D. */
static uint8_t transition[STATE][MASKS];
static uint8_t transcnt[STATE];
static bool     lastOk[STATE];
static bool     init_done = false;

/* Check whether the extension (U,C,D) satisfies the local rule.
   For every cell the number of neighbors equal to 2 must equal the
   cell's value; this is equivalent to:
   - if the cell itself is 2 (cur==1) then the neighbour count==2
   - otherwise the neighbour count!=2. */
static bool isValid(int U, int C, int D)
{
    for (int c = 0; c < 6; ++c) {
        int up    = (U >> c) & 1;
        int left  = (c > 0)    ? ((C >> (c-1)) & 1) : 0;
        int right = (c < 5)    ? ((C >> (c+1)) & 1) : 0;
        int cur   = (C >> c) & 1;
        int down  = (D >> c) & 1;

        int sum = up + left + right + down;
        if ((cur == 1 && sum != 2) || (cur == 0 && sum == 2))
            return false;
    }
    return true;
}

/* Precompute all transitions and the flag indicating that a state
   can be terminated by a zero row below. */
static void init(void)
{
    for (int U = 0; U < MASKS; ++U) {
        for (int C = 0; C < MASKS; ++C) {
            int idx = U * MASKS + C;
            uint8_t cnt = 0;
            bool okZero = false;
            for (int D = 0; D < MASKS; ++D) {
                if (isValid(U, C, D)) {
                    transition[idx][cnt++] = (uint8_t)D;
                    if (D == 0)
                        okZero = true;
                }
            }
            transcnt[idx] = cnt;
            lastOk[idx]   = okZero;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init();

    /* Dynamic programming over rows.  dp[s] counts ways with last
       two rows (prev, curr) represented by state s = prev*64 + curr. */
    static seq_t dp[STATE];
    static seq_t nextdp[STATE];

    /* Reset dp */
    for (int s = 0; s < STATE; ++s)
        dp[s] = (seq_t)0;

    /* Initial row: prev is implicitly 0.  All masks for first row are allowed. */
    for (int M1 = 0; M1 < MASKS; ++M1) {
        int s = M1;                /* state = 0*64 + M1 */
        dp[s] = (seq_t)1;
    }

    /* Process rows 2 .. n */
    for (int row = 2; row <= n; ++row) {
        /* clear nextdp */
        for (int s = 0; s < STATE; ++s)
            nextdp[s] = (seq_t)0;

        for (int s = 0; s < STATE; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0)
                continue;
            uint8_t curMask = (uint8_t)(s & 63);      /* current mask */
            uint8_t *list   = transition[s];
            uint8_t cnt     = transcnt[s];
            for (uint8_t j = 0; j < cnt; ++j) {
                int nextState = curMask * MASKS + list[j];
                nextdp[nextState] += val;
            }
        }
        /* swap dp and nextdp */
        for (int s = 0; s < STATE; ++s)
            dp[s] = nextdp[s];
    }

    /* Finalize: require down = 0 for the last row. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE; ++s) {
        if (dp[s] != (seq_t)0 && lastOk[s])
            result += dp[s];
    }
    return result;
}
