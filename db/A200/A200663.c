/*
 * A200663  Number of 0..3 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its three previous neighbors modulo 4.
 * https://oeis.org/A200663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A200663
    a(n) = number of 0..3 arrays x[0..n-1] where for each i≥1
    x[i] ≥ (x[i-1] + x[i-2] + x[i-3]) mod 4,
    with missing previous elements treated as 0.
    This implementation uses a DP with state (x[i-3],x[i-2],x[i-1]).
    Initial state is (0,0,0).  Complexity: O(n·64·4). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* DP arrays: cur holds counts for states after k elements,
       next holds counts for states after k+1 elements.            */
    seq_t cur[64] = {0}, next[64];
    cur[0] = 1;                          /* state (0,0,0) */

    for (idx_t step = 0; step < n; ++step) {
        /* clear next */
        for (int i = 0; i < 64; ++i) next[i] = 0;

        /* transition from all current states */
        for (int s = 0; s < 64; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;

            int a = s >> 4;          /* x[i-3] */
            int b = (s >> 2) & 3;    /* x[i-2] */
            int c = s & 3;           /* x[i-1] */

            int sum = (a + b + c) & 3;   /* modulo 4 */

            for (int d = sum; d < 4; ++d) {   /* x[i]  */
                int newidx = (b << 4) | (c << 2) | d;   /* state (b,c,d) */
                next[newidx] += cnt;
            }
        }

        /* swap cur and next for next iteration */
        for (int i = 0; i < 64; ++i) cur[i] = next[i];
    }

    /* total sequences after n steps = sum over all states */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += cur[i];
    return total;
}
