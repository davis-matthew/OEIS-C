/*
 * A200834  Number of 0..4 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): number of arrays of length n+2 over {0..4} that contain
 * no two consecutive increases or two consecutive decreases.
 * Computed by a linear‑time dynamic programming over the current value
 * and the last step direction. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* dp[cur][val][last_dir] where last_dir:
     *   0 – no restriction (start or previous step was flat),
     *   1 – last step was an increase,
     *   2 – last step was a decrease. */
    seq_t cur[5][3];
    seq_t nxt[5][3];
    int v, ld, v2, dir;

    /* Initial state: position 0 can be any value 0..4, no restriction yet. */
    for (v = 0; v < 5; ++v) {
        cur[v][0] = 1;
        cur[v][1] = 0;
        cur[v][2] = 0;
    }

    /* Number of differences (steps) is n + 1 (positions 0..n+1). */
    for (idx_t step = 0; step < n + 1; ++step) {
        /* Reset next array. */
        for (v = 0; v < 5; ++v)
            for (ld = 0; ld < 3; ++ld)
                nxt[v][ld] = 0;

        /* Transition from current state. */
        for (v = 0; v < 5; ++v) {
            for (ld = 0; ld < 3; ++ld) {
                seq_t cnt = cur[v][ld];
                if (cnt == 0)
                    continue;

                /* Try all possible next values. */
                for (v2 = 0; v2 < 5; ++v2) {
                    if (v2 > v)
                        dir = 1;   /* increase */
                    else if (v2 < v)
                        dir = 2;   /* decrease */
                    else
                        dir = 0;   /* flat */

                    /* Disallow two consecutive increases or decreases. */
                    if ((ld == 1 && dir == 1) || (ld == 2 && dir == 2))
                        continue;

                    nxt[v2][dir] += cnt;
                }
            }
        }

        /* Move to next step. */
        for (v = 0; v < 5; ++v)
            for (ld = 0; ld < 3; ++ld)
                cur[v][ld] = nxt[v][ld];
    }

    /* Sum all final states to get the total number of valid arrays. */
    seq_t result = 0;
    for (v = 0; v < 5; ++v)
        for (ld = 0; ld < 3; ++ld)
            result += cur[v][ld];

    return result;
}
