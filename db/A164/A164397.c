/*
 * A164397  Number of binary strings of length n with no substrings equal to
 *          0001 or 0111.
 * https://oeis.org/A164397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the number of binary strings of length n with no
 * substrings equal to 0001 or 0111.
 * The implementation uses a small DFA with 6 transient states
 * (empty, 0, 00, 01, 000, 011).  Transitions that would produce a
 * forbidden substring terminate in a dead state and are simply omitted.
 * The count is accumulated by dynamic programming in O(n) time. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* DFA transition table:
     * state 0: ""      - empty suffix
     * state 1: "0"
     * state 2: "00"
     * state 3: "01"
     * state 4: "000"
     * state 5: "011"
     * a value of -1 indicates a transition to the forbidden state. */
    static const int trans[6][2] = {
        { 1, 0 },   /* state 0 */
        { 2, 3 },   /* state 1 */
        { 4, 3 },   /* state 2 */
        { 1, 5 },   /* state 3 */
        { 4, -1 },  /* state 4 */
        { 1, -1 }   /* state 5 */
    };

    seq_t curr[6] = { 0 }, next[6];
    curr[0] = 1;   /* empty string is counted initially */

    for (idx_t i = 0; i < n; ++i) {
        for (int k = 0; k < 6; ++k) next[k] = 0;
        for (int s = 0; s < 6; ++s) {
            seq_t val = curr[s];
            if (!val) continue;
            int ns = trans[s][0];
            if (ns >= 0) next[ns] += val;
            ns = trans[s][1];
            if (ns >= 0) next[ns] += val;
        }
        for (int k = 0; k < 6; ++k) curr[k] = next[k];
    }

    seq_t total = 0;
    for (int k = 0; k < 6; ++k) total += curr[k];
    return total;
}
