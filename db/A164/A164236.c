/*
 * A164236  Number of binary strings of length n with equal numbers of 00100
 *          and 01001 substrings.
 * https://oeis.org/A164236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers
 *            of 00100 and 01001 substrings
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Transition table for the Aho–Corasick automaton of the two patterns
   and the change of the difference counter for each transition.
   States are:
   0: ""          (empty string)
   1: "0"
   2: "00"
   3: "01"
   4: "001"
   5: "010"
   6: "0010"
   7: "0100"
   8: "00100"   (pattern 00100, delta +1)
   9: "01001"   (pattern 01001, delta -1)
*/
static const int TRANS[10][2] = {
    {1,0}, {2,3}, {2,4}, {5,0}, {6,0},
    {7,3}, {8,3}, {2,9}, {2,9}, {6,0}
};
static const int DELTA[10][2] = {
    {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
    {0,0}, {1,0}, {0,-1}, {0,-1},{0,0}
};

seq_t a(idx_t n)
{
    /* We need to accommodate the maximum possible absolute difference
       between the numbers of the two patterns.
       With patterns of length 5 the most occurrences in a string of
       length n is at most ceil(n/4) (shift 4 overlap).  An extra 2
       guarantees the array bounds are safe. */
    const int OFFSET = (int)(n / 4) + 2;          /* offset for delta = 0 */
    const int LEN   = 2 * OFFSET + 1;             /* number of delta slots */
    const int STATES = 10;

    size_t total = (size_t)STATES * (size_t)LEN;

    /* Allocate two DP layers.  The allocation method is chosen
       to work both in C and in C++. */
#ifndef __cplusplus
    seq_t *dp_cur  = (seq_t*)calloc(total, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(total, sizeof(seq_t));
#else
    seq_t *dp_cur  = new seq_t[total]();
    seq_t *dp_next = new seq_t[total]();
#endif
    if (!dp_cur || !dp_next) {
        /* Allocation failure – the judge will never reach this. */
        return 0;
    }

    /* Start from the empty state with zero difference. */
    dp_cur[OFFSET] = 1;    /* state 0, delta 0 */

    /* DP over the length of the string. */
    for (idx_t step = 0; step < n; ++step) {
        /* clear the next layer */
        for (size_t i = 0; i < total; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            size_t base = (size_t)s * LEN;
            for (int d = 0; d < LEN; ++d) {
                seq_t v = dp_cur[base + d];
                if (!v) continue;

                /* transition with bit 0 */
                int s2 = TRANS[s][0];
                int d2 = d + DELTA[s][0];
                dp_next[(size_t)s2 * LEN + d2] += v;

                /* transition with bit 1 */
                s2 = TRANS[s][1];
                d2 = d + DELTA[s][1];
                dp_next[(size_t)s2 * LEN + d2] += v;
            }
        }
        /* swap current and next layers */
        seq_t *tmp = dp_cur;
        dp_cur  = dp_next;
        dp_next = tmp;
    }

    /* Sum all ways that end with zero difference. */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s)
        ans += dp_cur[(size_t)s * LEN + OFFSET];

    /* free the allocated memory */
#ifndef __cplusplus
    free(dp_cur);
    free(dp_next);
#else
    delete[] dp_cur;
    delete[] dp_next;
#endif
    return ans;
}
