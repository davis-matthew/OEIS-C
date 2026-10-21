/*
 * A229007  Number of arrays of median of three adjacent elements of some
 *          length n+2 0..3 array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct median sequences of length n that
 *            arise from length n+2 arrays over {0,1,2,3} with no adjacent
 *            equal elements. Computed using a deterministic automaton that
 *            tracks all possible current states (pairs of consecutive
 *            elements). The number of accepted words of length n is th...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   build_automaton
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/build_automaton.h"

seq_t a(idx_t n)
{
    static int initialized = 0;
    static uint16_t trans[12][4];
    static uint16_t nextMask[4096][4];
    static uint16_t state_id[4][4];
    static seq_t dp[4096];
    static seq_t next[4096];

    if (!initialized) {
        build_automaton(trans, nextMask, state_id);
        initialized = 1;
    }

    /* Initialize DP arrays */
    seq_t zero = 0;
    for (int i = 0; i < 4096; ++i)
        dp[i] = zero;

    uint16_t startMask = (1u << 12) - 1;        /* all 12 states reachable initially */
    dp[startMask] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 4096; ++i)
            next[i] = zero;

        for (int s = 0; s < 4096; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            for (int a = 0; a < 4; ++a) {
                uint16_t ns = nextMask[s][a];
                if (ns == 0) continue;
                next[ns] += ways;
            }
        }

        /* Copy next to dp for the next iteration */
        for (int i = 0; i < 4096; ++i)
            dp[i] = next[i];
    }

    seq_t result = 0;
    for (int i = 0; i < 4096; ++i)
        result += dp[i];

    return result;
}
