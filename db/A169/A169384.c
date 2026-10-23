/*
 * A169384  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   sum_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169384 – Number of reduced words of length n in the Coxeter
 * group on 35 generators S_i with relations (S_i)^2 = I and
 * (S_i S_j)^31 = I.
 *
 * Recurrence: let cur[l] be the number of reduced words of the current
 * length ending with an alternating run of length l (1 ≤ l ≤ 30).
 * For each new position we either
 *   • pick a new generator different from the previous one (run length resets to 1);
 *     there are 33 such choices;
 *   • or repeat the previous generator (run length increases by 1),
 *     unless the run would reach 31, which is forbidden.
 *
 * After n ≥ 2 positions the total number of reduced words is
 *   a(n) = 35·34·∑_{l=1}^{30} cur[l].
 *
 * The algorithm runs in O(n) time and O(1) memory (two arrays of
 * length 31).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_seq.h"

/* Number of generators and maximal allowed run length. */
static const idx_t K = 35;
static const int MAX_RUN = 30;   /* 31 − 1 */

seq_t a(idx_t n)
{
    /* Offset 0 and 1 are handled directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)K;     /* 35 */

    /* State arrays for runs of length 1 … 30.  Index 0 unused. */
    seq_t cur[MAX_RUN + 1];
    seq_t next[MAX_RUN + 1];

    /* Initialise to zero. */
    for (int i = 0; i <= MAX_RUN; ++i) cur[i] = 0;

    /* Length 2 base case: only runs of length 1 are possible. */
    cur[1] = (seq_t)1;

    if (n == 2) {
        seq_t s = cur[1];            /* sum of current runs */
        return (seq_t)(K * (K - 1)) * s;   /* 35·34·1 = 1190 */
    }

    /* Build successive lengths 3 … n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero next state. */
        for (int i = 0; i <= MAX_RUN; ++i) next[i] = 0;

        /* Total of all current runs. */
        seq_t sum = sum_seq(cur + 1, MAX_RUN);   /* cur[1] … cur[30] */

        /* New letter different from the previous one → run length 1. */
        next[1] = sum * (seq_t)(K - 2);          /* 33 choices */

        /* Transition where we repeat the previous letter. */
        for (int l = 1; l < MAX_RUN; ++l) {
            next[l + 1] = cur[l];
        }

        /* Copy next → cur for the next iteration. */
        for (int i = 1; i <= MAX_RUN; ++i) cur[i] = next[i];
    }

    /* Final result for length n. */
    seq_t total = sum_seq(cur + 1, MAX_RUN);
    return (seq_t)(K * (K - 1)) * total;   /* 35·34·∑cur[l] */
}
