/*
 * A118425  Number of binary sequences of length n containing exactly one
 *          subsequence 001.
 * https://oeis.org/A118425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n containing exactly *
 *            one occurrence of the substring 001. * * The sequence is
 *            computed with a linear‑time DP that tracks: * - how many matches
 *            of 001 have been seen so far (0 or 1), * - the longest suffix of
 *            the current string that is a prefix of 001 *...
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* dp0[l] – number of strings with 0 matches and suffix length l
     * dp1[l] – number of strings with 1 match  and suffix length l
     */
    seq_t dp0[3] = {0, 0, 0};
    seq_t dp1[3] = {0, 0, 0};
    dp0[0] = 1;          /* empty string: 0 matches, empty suffix */

    for (idx_t i = 0; i < n; ++i) {
        seq_t new0[3] = {0, 0, 0};
        seq_t new1[3] = {0, 0, 0};

        /* Transition from state 0 (no matches yet) */
        /* l = 0 */
        new0[1] += dp0[0];   /* add bit 0 -> suffix "0" */
        new0[0] += dp0[0];   /* add bit 1 -> suffix "" */
        /* l = 1 */
        new0[2] += dp0[1];   /* add bit 0 -> suffix "00" */
        new0[0] += dp0[1];   /* add bit 1 -> suffix "" */
        /* l = 2 */
        new0[2] += dp0[2];   /* add bit 0 -> suffix "00" */
        new1[0] += dp0[2];   /* add bit 1 -> one match found */

        /* Transition from state 1 (one match already seen) */
        /* l = 0 */
        new1[1] += dp1[0];   /* add bit 0 -> suffix "0" */
        new1[0] += dp1[0];   /* add bit 1 -> suffix "" */
        /* l = 1 */
        new1[2] += dp1[1];   /* add bit 0 -> suffix "00" */
        new1[0] += dp1[1];   /* add bit 1 -> suffix "" */
        /* l = 2 */
        new1[2] += dp1[2];   /* add bit 0 -> suffix "00" */
        /* add bit 1 would create a second match – discard it */

        /* store new state */
        dp0[0] = new0[0]; dp0[1] = new0[1]; dp0[2] = new0[2];
        dp1[0] = new1[0]; dp1[1] = new1[1]; dp1[2] = new1[2];
    }

    /* Sum all states with exactly one match */
    return dp1[0] + dp1[1] + dp1[2];
}
