/*
 * A164476  Number of binary strings of length n with no substrings equal to
 *          0001, 0110, or 0111.
 * https://oeis.org/A164476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 71, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Uses a deterministic finite automaton to count binary strings of length n
 * that avoid the forbidden substrings 0001, 0110, and 0111.
 * The automaton has 6 states:
 *   0: ""          (empty suffix)
 *   1: "0"
 *   2: "00"
 *   3: "000"
 *   4: "01"
 *   5: "011"
 * Transitions are defined by appending 0 or 1 to the current suffix
 * and taking the longest suffix that is a prefix of a forbidden pattern.
 * State 5 has no outgoing transitions because any further symbol
 * would produce a forbidden pattern.
 */

#include <stdint.h>
/* seq_t and idx_t are defined by the build system */

seq_t a(idx_t n)
{
    /* For n = 0 the empty string is allowed */
    if (n == 0)
        return 1;

    /* DP arrays for current and next step */
    seq_t cur[6] = {0};
    seq_t nxt[6];
    /* initial state: empty suffix with count 1 */
    cur[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* reset next state */
        nxt[0] = nxt[1] = nxt[2] = nxt[3] = nxt[4] = nxt[5] = 0;

        /* state 0: "" */
        if (cur[0]) {
            nxt[1] += cur[0];  /* append 0 -> state 1 */
            nxt[0] += cur[0];  /* append 1 -> state 0 */
        }

        /* state 1: "0" */
        if (cur[1]) {
            nxt[2] += cur[1];  /* append 0 -> state 2 */
            nxt[4] += cur[1];  /* append 1 -> state 4 */
        }

        /* state 2: "00" */
        if (cur[2]) {
            nxt[3] += cur[2];  /* append 0 -> state 3 */
            nxt[4] += cur[2];  /* append 1 -> state 4 */
        }

        /* state 3: "000" */
        if (cur[3]) {
            nxt[3] += cur[3];  /* append 0 -> state 3 */
            /* append 1 leads to forbidden 0001: skip */
        }

        /* state 4: "01" */
        if (cur[4]) {
            nxt[1] += cur[4];  /* append 0 -> state 1 */
            nxt[5] += cur[4];  /* append 1 -> state 5 */
        }

        /* state 5: "011" has no outgoing transitions (all lead to forbidden) */

        /* copy next to current for next iteration */
        cur[0] = nxt[0]; cur[1] = nxt[1]; cur[2] = nxt[2];
        cur[3] = nxt[3]; cur[4] = nxt[4]; cur[5] = nxt[5];
    }

    /* Sum counts over all non-terminal states */
    seq_t total = 0;
    total += cur[0];
    total += cur[1];
    total += cur[2];
    total += cur[3];
    total += cur[4];
    total += cur[5];
    return total;
}
