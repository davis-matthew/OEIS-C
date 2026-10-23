/*
 * A164456  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 1000.
 * https://oeis.org/A164456
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164456: Number of binary strings of length n with no substrings 0001, 0011 or 1000. */
seq_t a(idx_t n)
{
    /* Transition table for the DFA that avoids the forbidden patterns.
       States are indexed as follows:
          0: ""      (empty string)
          1: "0"
          2: "1"
          3: "00"
          4: "01"
          5: "10"
          6: "11"
          7: "000"
          8: "001"
          9: "100"
       A transition to -1 means the resulting 4‑bit block is forbidden.
    */
    static const int next_state[10][2] = {
        /* 0: ""      */ { 1,  2},
        /* 1: "0"     */ { 3,  4},
        /* 2: "1"     */ { 5,  6},
        /* 3: "00"    */ { 7,  8},
        /* 4: "01"    */ { 5,  6},
        /* 5: "10"    */ { 9,  4},
        /* 6: "11"    */ { 5,  6},
        /* 7: "000"   */ { 7, -1},
        /* 8: "001"   */ { 5, -1},
        /* 9: "100"   */ {-1,  8}
    };

    /* DP arrays for current and next state counts. */
    seq_t cur[10] = {0};
    seq_t nxt[10] = {0};

    /* Start with the empty prefix. */
    cur[0] = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* Reset next-state counts. */
        for (int s = 0; s < 10; ++s) nxt[s] = 0;

        /* Propagate counts through the transitions. */
        for (int s = 0; s < 10; ++s) {
            if (cur[s] == 0) continue;

            int ns0 = next_state[s][0];
            if (ns0 >= 0) nxt[ns0] += cur[s];

            int ns1 = next_state[s][1];
            if (ns1 >= 0) nxt[ns1] += cur[s];
        }

        /* Move to the next position. */
        for (int s = 0; s < 10; ++s) cur[s] = nxt[s];
    }

    /* Sum all valid ending states. */
    seq_t total = 0;
    for (int s = 0; s < 10; ++s) total += cur[s];
    return total;
}
