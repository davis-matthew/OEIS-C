/*
 * A239025  Number of n X 3 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of elements above it,
 *          modulo 3.
 * https://oeis.org/A239025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} with no element * equal
 *            to the sum of elements to its left or one plus the sum of *
 *            elements above it (mod 3). The algorithm builds a 27×27 transfer
 *            * matrix T indexed by column‑sum states (s0,s1,s2)∈{0,1,2}³. *
 *            Iterating n times with the vector dp (dp[old...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----------- Build the transfer matrix once --------------------- */
    static seq_t T[27][27];
    static bool built = false;

    if (!built) {
        /* zero the matrix */
        for (idx_t i = 0; i < 27; ++i)
            for (idx_t j = 0; j < 27; ++j)
                T[i][j] = (seq_t)0;

        /* all possible old states */
        for (idx_t old = 0; old < 27; ++old) {
            idx_t s0 = old % 3;
            idx_t tmp = old / 3;
            idx_t s1 = tmp % 3;
            idx_t s2 = tmp / 3;

            /* choose a row (r0,r1,r2) respecting the constraints */
            for (idx_t r0 = 0; r0 < 3; ++r0) {
                /* left sum for column 0 is 0 */
                if (r0 == 0)                 /* cannot equal left sum     */
                    continue;
                if (r0 == (s0 + 1) % 3)      /* cannot equal above+1      */
                    continue;

                for (idx_t r1 = 0; r1 < 3; ++r1) {
                    idx_t left1 = r0 % 3;    /* left sum for column 1 */
                    if (r1 == left1)
                        continue;
                    if (r1 == (s1 + 1) % 3)
                        continue;

                    for (idx_t r2 = 0; r2 < 3; ++r2) {
                        idx_t left2 = (r0 + r1) % 3;/* left sum col 2 */
                        if (r2 == left2)
                            continue;
                        if (r2 == (s2 + 1) % 3)
                            continue;

                        /* transition is allowed – compute new state */
                        idx_t new_s0 = (s0 + r0) % 3;
                        idx_t new_s1 = (s1 + r1) % 3;
                        idx_t new_s2 = (s2 + r2) % 3;
                        idx_t new_id = new_s0 + 3 * new_s1 + 9 * new_s2;

                        T[old][new_id] += (seq_t)1;
                    }
                }
            }
        }
        built = true;
    }

    /* ----------- Dynamic programming over the rows ----------------- */
    seq_t dp[27]  = {0};
    seq_t ndp[27] = {0};
    dp[0] = (seq_t)1;  /* start with column sums all zero */

    for (idx_t step = 0; step < n; ++step) {
        /* clear ndp */
        for (idx_t i = 0; i < 27; ++i)
            ndp[i] = (seq_t)0;

        for (idx_t old = 0; old < 27; ++old) {
            if (dp[old] == (seq_t)0)
                continue;
            for (idx_t tnext = 0; tnext < 27; ++tnext) {
                if (T[old][tnext] == (seq_t)0)
                    continue;
                ndp[tnext] += dp[old] * T[old][tnext];
            }
        }

        /* copy ndp back into dp */
        for (idx_t i = 0; i < 27; ++i)
            dp[i] = ndp[i];
    }

    /* sum all final states – this is a(n) */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 27; ++i)
        total += dp[i];

    return total;
}
