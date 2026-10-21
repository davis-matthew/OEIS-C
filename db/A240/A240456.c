/*
 * A240456  Number of nX2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or three plus the sum of elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   decode_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by DP over the rows of an n×2 array with
 * elements in {0,1,2,3} under the following constraints (for each cell):
 *
 *   • v ≠ (1 + sum_of_elements_to_its_left) mod 4
 *   • v ≠ (3 + sum_of_elements_above_it) mod 4
 *   • v ≠ (1 + sum_of_elements_diagonally_northwest) mod 4
 *
 * For a 2‑column array the sums are:
 *   – left sum for column 0 is 0, for column 1 it is the cell to its left.
 *   – above sum is the sum of the same column in all preceding rows (taken mod 4).
 *   – diag‑NW sum for column 1 is the cell in the previous row, column 0
 *     (0 if the previous row does not exist).  Column 0 has no NW neighbour.
 *
 * The DP keeps for each row a triple (s0,s1,p0):
 *   s0 – cumulative sum of column 0 modulo 4 up to the previous row,
 *   s1 – cumulative sum of column 1 modulo 4 up to the previous row,
 *   p0 – value of column 0 in the previous row (used for the diagonal).
 * There are 4×4×4 = 64 states.
 *
 * Transitions: for a given state (s0,s1,p0) and a choice of current
 * values (a,b) with the above constraints, the next state is
 *   ( (s0+a) mod 4 , (s1+b) mod 4 , a ).
 *
 * The number of sequences equals the sum of counts over all states after
 * processing n rows.  The code recomputes the DP for every call; the
 * complexity is O(n * 64) which is far below the required time limit.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/decode_state.h"

#define N_STATES 64

/* Encode a state (s0,s1,p0) into [0,63] */
static inline size_t encode_state(uint8_t s0, uint8_t s1, uint8_t p0)
{
    return (size_t)s0 * 16 + (size_t)s1 * 4 + (size_t)p0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for the offset, safe return */

    static seq_t dp[N_STATES];
    static seq_t next[N_STATES];

    /* initial state: sums are 0, previous left value is 0 (diagonal is 0) */
    for (size_t i = 0; i < N_STATES; ++i)
        dp[i] = 0;
    dp[0] = 1;           /* encode_state(0,0,0) == 0 */

    for (idx_t row = 0; row < n; ++row) {
        for (size_t i = 0; i < N_STATES; ++i)
            next[i] = 0;

        for (size_t st = 0; st < N_STATES; ++st) {
            seq_t ways = dp[st];
            if (!ways)
                continue;

            uint8_t s0, s1, p0;
            decode_state(st, &s0, &s1, &p0);

            for (uint8_t a_val = 0; a_val < 4; ++a_val) {
                /* constraints for column 0 */
                if (a_val == 1)                       /* left sum is 0 */
                    continue;
                if (a_val == (3 + s0) % 4)            /* above sum */
                    continue;

                for (uint8_t b_val = 0; b_val < 4; ++b_val) {
                    /* constraints for column 1 */
                    if (b_val == (1 + a_val) % 4)     /* left in same row */
                        continue;
                    if (b_val == (3 + s1) % 4)        /* above in column 1 */
                        continue;
                    if (b_val == (1 + p0) % 4)        /* diagonal NW */
                        continue;

                    uint8_t ns0 = (s0 + a_val) & 3;
                    uint8_t ns1 = (s1 + b_val) & 3;
                    size_t nst = encode_state(ns0, ns1, a_val);
                    next[nst] += ways;
                }
            }
        }

        /* swap dp and next */
        for (size_t i = 0; i < N_STATES; ++i)
            dp[i] = next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (size_t i = 0; i < N_STATES; ++i)
        total += dp[i];

    return total;
}
