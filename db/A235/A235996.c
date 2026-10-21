/*
 * A235996  Number of length n binary words that contain at least one pair of
 *          consecutive 0's followed by (at some point in the word) at least
 *          one pair of consecutive 1's.
 * https://oeis.org/A235996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235996: number of binary words of length n that contain
   at least one pair of consecutive 0's followed by (possibly later)
   at least one pair of consecutive 1's.  The algorithm uses a
   5‑state automaton that tracks whether the 00 has been seen and, if
   so, whether a 11 has followed it.  The counts are accumulated
   iteratively in O(n) time.  Initial values:
   a(0) = a(1) = a(2) = a(3) = 0, a(4) = 1, a(5) = 4. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* State encoding:
       0 – no 00 seen yet, last bit 0
       1 – no 00 seen yet, last bit 1
       2 – 00 seen, last bit 0, no 11 seen after that 00
       3 – 00 seen, last bit 1, no 11 seen after that 00
       4 – condition already satisfied (00 before 11 seen)
    */
    static const int trans[5][2] = {
        { 2, 1 }, /* state 0 (last 0) */
        { 0, 1 }, /* state 1 (last 1) */
        { 2, 3 }, /* state 2 (00 seen, last 0) */
        { 2, 4 }, /* state 3 (00 seen, last 1) */
        { 4, 4 }  /* state 4 (absorbing) */
    };

    seq_t curr[5] = { 0, 0, 0, 0, 0 };
    seq_t next[5] = { 0, 0, 0, 0, 0 };

    /* Initialise after processing the first bit:
       first bit 0 → state 0, first bit 1 → state 1   */
    curr[0] = 1;
    curr[1] = 1;

    for (idx_t pos = 1; pos < n; ++pos) {
        /* reset next */
        for (int i = 0; i < 5; ++i)
            next[i] = 0;

        for (int s = 0; s < 5; ++s) {
            seq_t val = curr[s];
            if (val == 0)
                continue;

            /* append bit 0 */
            int ns0 = trans[s][0];
            next[ns0] += val;

            /* append bit 1 */
            int ns1 = trans[s][1];
            next[ns1] += val;
        }

        /* move to next position */
        for (int i = 0; i < 5; ++i)
            curr[i] = next[i];
    }

    /* result is the number of words that satisfy the condition
       (state 4) after n bits. */
    return curr[4];
}
