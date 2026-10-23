/*
 * A164449  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 1010.
 * https://oeis.org/A164449
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164449: Number of binary strings of length n with no substrings 0001, 0010, or 1010.
   Computation uses a small DFA with 10 states (the longest suffix which is a prefix of
   any forbidden word).  For each bit added we transition to the next state if the
   resulting 4‑bit pattern is not forbidden; otherwise the transition is discarded.
   The DP counts sequences of a given length that end in each state.  The sum over
   all states gives a(n).  Complexity: O(n) per call with 10 states, trivial memory. */

seq_t a(idx_t n)
{
    /* Number of DFA states: 10 */
    static const int TRANS[10][2] = {
        { 1,  2},  /* ""      -> 0,1 */
        { 3,  4},  /* "0"     -> 00,01 */
        { 5,  2},  /* "1"     -> 10,1 */
        { 6,  7},  /* "00"    -> 000,001 */
        { 8,  2},  /* "01"    -> 010,1 */
        { 3,  9},  /* "10"    -> 100,101 */
        { 6, -1},  /* "000"   -> 000, forb. */
        {-1,  2},  /* "001"   -> forb.,1 */
        { 3,  9},  /* "010"   -> 100,101 */
        {-1,  2}   /* "101"   -> forb.,1 */
    };
    seq_t dp[10]      = {0};
    seq_t next[10]    = {0};
    seq_t result      = 0;

    dp[0] = 1;  /* start with empty suffix */

    for (idx_t i = 0; i < n; ++i) {
        /* zero the next array */
        for (int s = 0; s < 10; ++s) next[s] = 0;

        for (int s = 0; s < 10; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int bit = 0; bit < 2; ++bit) {
                int ns = TRANS[s][bit];
                if (ns < 0) continue;         /* forbidden transition */
                next[ns] = next[ns] + cur;     /* accumulate counts */
            }
        }
        /* swap dp and next for next iteration */
        for (int s = 0; s < 10; ++s) dp[s] = next[s];
    }

    /* sum all states to get total number of valid strings of length n */
    for (int s = 0; s < 10; ++s) result = result + dp[s];

    return result;
}
