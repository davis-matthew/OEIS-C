/*
 * A164419  Number of binary strings of length n with no substrings equal to
 *          0000, 0010, or 0110.
 * https://oeis.org/A164419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n avoiding the
 *            substrings 0000, 0010, and 0110
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a 7‑state DFA derived from the forbidden
   patterns and dynamic programming in O(n).  */

seq_t a(idx_t n)
{
    /* DFA states:
       0: ""      (empty prefix)
       1: "0"
       2: "00"
       3: "000"
       4: "001"
       5: "01"
       6: "011"
       Transition tables: -1 indicates failure (forbidden pattern reached). */
    static const int trans0[7] = { 1, 2, 3, -1, -1, 1, -1 };
    static const int trans1[7] = { 0, 5, 4, 4, 6, 6, 0 };

    /* Current and next DP arrays.  seq_t is an unsigned integer type. */
    seq_t cur[7] = {0};
    seq_t nxt[7];
    cur[0] = 1;                  /* empty string of length 0 starts at state 0 */

    for (idx_t i = 0; i < n; ++i) {
        /* zero next array */
        for (int s = 0; s < 7; ++s) {
            nxt[s] = 0;
        }
        /* propagate transitions */
        for (int s = 0; s < 7; ++s) {
            seq_t val = cur[s];
            if (!val) continue;

            int t0 = trans0[s];
            if (t0 >= 0) nxt[t0] += val;

            int t1 = trans1[s];
            if (t1 >= 0) nxt[t1] += val;
        }
        /* copy next to cur */
        for (int s = 0; s < 7; ++s) {
            cur[s] = nxt[s];
        }
    }

    /* Sum over all non‑failure states to get the answer. */
    seq_t ans = 0;
    for (int s = 0; s < 7; ++s) {
        ans += cur[s];
    }
    return ans;
}
