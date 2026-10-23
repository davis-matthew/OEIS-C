/*
 * A164399  Number of binary strings of length n with no substrings equal to
 *          0001 or 1010.
 * https://oeis.org/A164399
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   Count binary strings of length n that avoid the patterns 0001
 *            and 1010. This is implemented by a DFA with 15 states that
 *            remember the current length (0..3) and the last up to three bits
 *            of the string
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Transition table: next_state[state][bit] = next state id, or -1 if the
   transition creates a forbidden substring.  The states are indexed as
   follows:
   0: len=0, val=0
   1: len=1, val=0
   2: len=1, val=1
   3: len=2, val=0
   4: len=2, val=1
   5: len=2, val=2
   6: len=2, val=3
   7: len=3, val=0  (000)
   8: len=3, val=1  (001)
   9: len=3, val=2  (010)
  10: len=3, val=3  (011)
  11: len=3, val=4  (100)
  12: len=3, val=5  (101)  -- transition 0 is forbidden
  13: len=3, val=6  (110)
  14: len=3, val=7  (111) */
static const int next_state[15][2] = {
    {  1,  2 },   /* 0 */
    {  3,  4 },   /* 1 */
    {  5,  6 },   /* 2 */
    {  7,  8 },   /* 3 */
    {  9, 10 },   /* 4 */
    { 11, 12 },   /* 5 */
    { 13, 14 },   /* 6 */
    {  7, -1 },   /* 7 */
    {  9, 10 },   /* 8 */
    { 11, 12 },   /* 9 */
    { 13, 14 },   /*10 */
    {  7,  8 },   /*11 */
    { -1, 10 },   /*12 */
    { 11, 12 },   /*13 */
    { 13, 14 }    /*14 */
};

seq_t a(idx_t n)
{
    /* Values for n < 4 are outside the offset; return 0. */
    if (n < 4) return 0;

    seq_t dp[15]    = {0};
    seq_t nextdp[15]= {0};

    /* Start with the empty string (length 0). */
    dp[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* Clear the next state array. */
        for (int i = 0; i < 15; ++i) nextdp[i] = 0;

        /* Transition from every reachable state. */
        for (int i = 0; i < 15; ++i) {
            seq_t cnt = dp[i];
            if (cnt == 0) continue;

            int j0 = next_state[i][0];
            if (j0 >= 0) nextdp[j0] += cnt;

            int j1 = next_state[i][1];
            if (j1 >= 0) nextdp[j1] += cnt;
        }

        /* Copy nextdp back to dp for the next iteration. */
        for (int i = 0; i < 15; ++i) dp[i] = nextdp[i];
    }

    /* After n steps all strings of length n have length 3 suffixes
       (since n >= 4). Sum the counts of all states with len == 3. */
    seq_t result = 0;
    for (int i = 7; i < 15; ++i) result += dp[i];
    return result;
}
