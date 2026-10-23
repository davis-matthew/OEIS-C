/*
 * A118871  Number of binary sequences of length n containing exactly one
 *          subsequence 0101.
 * https://oeis.org/A118871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that contain exactly
 *            one occurrence of the substring 0101. The implementation uses
 *            dynamic programming on the automaton for the pattern 0101,
 *            keeping track of the current suffix state and whether a match
 *            has already been seen
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Automaton for the pattern 0101.
       States:
          0 – no prefix matched
          1 – matched "0"
          2 – matched "01"
          3 – matched "010"
       nextS[s][b] gives the new state after appending bit b.
       matched[s][b] is true iff appending bit b completes the pattern.          */
    static const int nextS[4][2] = {
        {1, 0},   /* from state 0 */
        {1, 2},   /* from state 1 */
        {3, 0},   /* from state 2 */
        {1, 2}    /* from state 3; bit 1 would match the pattern */
    };
    static const int matched[4][2] = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 1}    /* state 3, bit 1 completes 0101 */
    };

    /* DP arrays: cur[state][matches] and next[state][matches].
       matches: 0 – no match seen yet, 1 – exactly one match seen. */
    seq_t cur[4][2] = {{0}};
    seq_t next[4][2];
    cur[0][0] = 1;     /* empty string starts in state 0 with 0 matches */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero next array */
        for (int s = 0; s < 4; ++s) {
            next[s][0] = next[s][1] = 0;
        }

        for (int s = 0; s < 4; ++s) {
            for (int m = 0; m < 2; ++m) {
                seq_t val = cur[s][m];
                if (!val) continue;

                for (int bit = 0; bit < 2; ++bit) {
                    if (matched[s][bit]) {
                        if (m == 0) {
                            int ns = nextS[s][bit];
                            next[ns][1] += val;   /* first match occurs here */
                        }                       /* otherwise second match – discard */
                    } else {
                        int ns = nextS[s][bit];
                        next[ns][m] += val;       /* still have the same number of matches */
                    }
                }
            }
        }

        /* copy next→cur for the next iteration */
        for (int s = 0; s < 4; ++s) {
            cur[s][0] = next[s][0];
            cur[s][1] = next[s][1];
        }
    }

    /* Sum over all final states with exactly one match. */
    seq_t res = 0;
    for (int s = 0; s < 4; ++s)
        res += cur[s][1];

    return res;
}
