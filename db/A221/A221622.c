/*
 * A221622  Number of nX7 arrays with each row a permutation of 1..7 having at
 *          least as many downsteps as the preceding row.
 * https://oeis.org/A221622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221622: Number of n×7 arrays of permutations of 1..7 with
 * non‑decreasing number of downsteps.  Let c[d] be the number of
 * permutations of 1..7 that have exactly d descents (Eulerian numbers
 * for n = 7).  Then, if T(k,d) is the number of length‑k
 * sequences ending with a permutation having d descents,
 *   T(1,d) = c[d]
 *   T(k,d) = c[d] * Σ_{t=0..d} T(k‑1,t)    for k ≥ 2
 * The required value is
 *   a(n) = Σ_{d=0..6} T(n,d)
 *
 * Initial values (c[d]): 1, 120, 1191, 2416, 1191, 120, 1
 * (they sum to 7! = 5040).  This recurrence is evaluated in O(7·n)
 * time and uses O(7) space.
 */

#include <stdint.h>

static const seq_t Eulerian7[7] = {1, 120, 1191, 2416, 1191, 120, 1};

seq_t a(idx_t n)
{
    if (n < 1)    /* undefined below offset; return 0 */
        return 0;

    /* dp_prev[d] = T(k-1,d), dp_cur[d] = T(k,d) */
    seq_t dp_prev[7];
    seq_t dp_cur[7];
    seq_t pref[7];
    seq_t result;

    /* initial k = 1 */
    for (int d = 0; d < 7; ++d)
        dp_prev[d] = Eulerian7[d];

    /* iterate for k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* prefix sums of dp_prev */
        seq_t acc = 0;
        for (int d = 0; d < 7; ++d) {
            acc += dp_prev[d];
            pref[d] = acc;
        }
        /* compute dp_cur[d] = c[d] * pref[d] */
        for (int d = 0; d < 7; ++d)
            dp_cur[d] = Eulerian7[d] * pref[d];

        /* swap for next iteration */
        for (int d = 0; d < 7; ++d)
            dp_prev[d] = dp_cur[d];
    }

    /* sum over last row */
    result = 0;
    for (int d = 0; d < 7; ++d)
        result += dp_prev[d];

    return result;
}
