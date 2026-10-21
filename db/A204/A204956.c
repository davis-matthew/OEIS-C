/*
 * A204956  Number of (n+1)X3 0..1 arrays with rows and columns of permanents
 *          of all 2X2 subblocks lexicographically nondecreasing.
 * https://oeis.org/A204956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 binary arrays whose * 2×2 subblock
 *            permanents form a matrix with rows and columns *
 *            lexicographically nondecreasing. * * The algorithm counts all
 *            arrays by dynamic programming over * consecutive row pairs. Each
 *            state stores: * - the last row pattern (3 bits, 0…7) * - the...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define NUM_ROWS 8           /* all 3‑bit row patterns */
#define MAX_PERM 3          /* permanents 0..2 */
#define COL_STATE 2
#define NUM_STATES (NUM_ROWS * MAX_PERM * MAX_PERM * COL_STATE)  /* 144 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    static seq_t curr[NUM_STATES];
    static seq_t next[NUM_STATES];

    /* initialise all counters to zero */
    for (size_t i = 0; i < NUM_STATES; ++i) {
        curr[i] = 0;
        next[i] = 0;
    }

    /* --------- initial distribution for one pair (n = 1) --------- */
    for (int r0 = 0; r0 < NUM_ROWS; ++r0) {
        int a0 =  (r0 >> 0) & 1;
        int a1 =  (r0 >> 1) & 1;
        int a2 =  (r0 >> 2) & 1;
        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            int b0 =  (r1 >> 0) & 1;
            int b1 =  (r1 >> 1) & 1;
            int b2 =  (r1 >> 2) & 1;

            int p0 = a0 * b1 + a1 * b0;
            int p1 = a1 * b2 + a2 * b1;
            if (p0 > p1) continue;                     /* column lex fails */

            int col_state = (p0 < p1) ? 1 : 0;          /* 0 unresolved, 1 determined */
            size_t idx = ((( r1 * MAX_PERM + p0) * MAX_PERM + p1) * COL_STATE + col_state);
            curr[idx] += 1;
        }
    }

    /* compute a(1) */
    seq_t result = 0;
    for (size_t i = 0; i < NUM_STATES; ++i) result += curr[i];
    if (n == 1) return result;

    /* --------- iterate for n >= 2 --------- */
    for (idx_t step = 2; step <= n; ++step) {

        /* zero next array */
        for (size_t i = 0; i < NUM_STATES; ++i) next[i] = 0;

        /* process each state */
        for (size_t idx = 0; idx < NUM_STATES; ++idx) {
            seq_t count = curr[idx];
            if (count == 0) continue;

            /* decode state */
            size_t tmp = idx;
            int col_state = tmp % COL_STATE;   tmp /= COL_STATE;
            int p1 = tmp % MAX_PERM;           tmp /= MAX_PERM;
            int p0 = tmp % MAX_PERM;           tmp /= MAX_PERM;
            int r_prev = tmp;                  /* 0..7 */

            /* try all possible new rows */
            for (int r_new = 0; r_new < NUM_ROWS; ++r_new) {
                int c0 =  (r_new >> 0) & 1;
                int c1 =  (r_new >> 1) & 1;
                int c2 =  (r_new >> 2) & 1;

                int new_p0 = (r_prev >> 0 & 1) * c1 + (r_prev >> 1 & 1) * (r_new >> 0 & 1);
                int new_p1 = (r_prev >> 1 & 1) * c2 + (r_prev >> 2 & 1) * c1;

                /* row lex order constraint */
                if (new_p0 < p0) continue;
                if (new_p0 == p0 && new_p1 < p1) continue;

                /* column lex state */
                int new_col_state;
                if (col_state == 1) {
                    new_col_state = 1;                 /* already determined */
                } else {
                    if (new_p0 < new_p1) new_col_state = 1;
                    else if (new_p0 == new_p1) new_col_state = 0;
                    else continue;                    /* new_p0 > new_p1 => invalid */
                }

                size_t new_idx = ((( r_new * MAX_PERM + new_p0) * MAX_PERM + new_p1) * COL_STATE + new_col_state);
                next[new_idx] += count;
            }
        }

        /* compute a(step) as sum of next */
        seq_t sum = 0;
        for (size_t i = 0; i < NUM_STATES; ++i) sum += next[i];
        if (step == n) return sum;

        /* prepare for next iteration */
        for (size_t i = 0; i < NUM_STATES; ++i) curr[i] = next[i];
    }

    /* unreachable */
    return 0;
}
