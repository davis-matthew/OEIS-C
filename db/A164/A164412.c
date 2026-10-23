/*
 * A164412  Number of binary strings of length n with no substrings equal to
 *          0000 0001 or 0111.
 * https://oeis.org/A164412
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of binary strings of length n avoiding substrings
 *   '0000', '0001', and '0111'.  The automaton has 6 states for the
 *   longest suffix that is a prefix of a forbidden word. */

seq_t a(idx_t n)
{
    /* Off‑setting: sequence is defined only for n >= 4 */
    if (n < 4) return 0;

    /* Transition table: next_state[s][b] gives next state (0..5)
     *   -1 denotes a forbidden transition that must not be counted. */
    static const int next_state[6][2] = {
        /* 0 */ { 1, 0 },   /* empty  -> 0 -> 1, 1 -> 0 */
        /* 1 */ { 2, 4 },   /* 0   -> 2, 1 -> 4 */
        /* 2 */ { 3, 4 },   /* 00  -> 3, 1 -> 4 */
        /* 3 */ { -1, -1 }, /* 000 => forbidden on both 0 and 1 */
        /* 4 */ { 1, 5 },   /* 01  -> 1, 1 -> 5 */
        /* 5 */ { 1, -1 }   /* 011 -> 1, 1 -> forbidden */
    };

    /* DP arrays for current and next length */
    seq_t cur[6] = {0};
    seq_t nxt[6];

    /* Initial state (empty string of length 0) */
    cur[0] = 1;

    /* Iterate over string length */
    for (idx_t len = 0; len < n; ++len) {
        /* Zero next array */
        nxt[0] = nxt[1] = nxt[2] = nxt[3] = nxt[4] = nxt[5] = 0;

        /* Transition from each state */
        for (int s = 0; s < 6; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;

            int t0 = next_state[s][0];
            if (t0 != -1) nxt[t0] += val;

            int t1 = next_state[s][1];
            if (t1 != -1) nxt[t1] += val;
        }

        /* Copy next to current for next iteration */
        cur[0] = nxt[0];
        cur[1] = nxt[1];
        cur[2] = nxt[2];
        cur[3] = nxt[3];
        cur[4] = nxt[4];
        cur[5] = nxt[5];
    }

    /* Sum over all non‑forbidden states to get a(n) */
    seq_t sum = 0;
    sum += cur[0];
    sum += cur[1];
    sum += cur[2];
    sum += cur[3];
    sum += cur[4];
    sum += cur[5];
    return sum;
}
