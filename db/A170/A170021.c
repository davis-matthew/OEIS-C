/*
 * A170021  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of reduced words of length n in a Coxeter
 *            group with 12 generators and relations (S_i)^2 = I, (S_i S_j)^36
 *            = I. The recurrence is derived from the regular expression
 *            condition that no two consecutive generators may be equal and no
 *            alternating repetition of two distinct generato...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)132;

    /* B[k] : number of words of current length ending with two
       distinct generators and alternation count k (0 ≤ k ≤ 35) */
    seq_t B[36] = {0};
    seq_t newB[36];
    /* Length 2: all ordered pairs of distinct generators */
    B[0] = (seq_t)132;  /* 12 choices for first, 11 for second */

    for (idx_t len = 3; len <= n; ++len) {
        /* Zero newB */
        for (int k = 0; k < 36; ++k) newB[k] = (seq_t)0;

        for (int k = 0; k < 36; ++k) {
            seq_t cnt = B[k];
            if (cnt == (seq_t)0) continue;

            /* Choose a new generator different from the last one.
               10 choices leave alternation count at 0. */
            newB[0] += cnt * (seq_t)10;

            if (k == 0) {
                /* From alternation count 0, one choice continues
                   alternation to count 1. */
                newB[1] += cnt;
            } else if (k < 35) {
                /* For 1 ≤ k ≤ 34, one choice continues alternation
                   to count k+1.  The choice that would reach 36
                   (k = 35) is forbidden and therefore omitted. */
                newB[k + 1] += cnt;
            }
            /* No contribution for k = 35 that would create 36 pairs. */
        }

        /* Copy newB to B for next iteration */
        for (int k = 0; k < 36; ++k) B[k] = newB[k];
    }

    /* Sum all counts for the final length */
    seq_t total = (seq_t)0;
    for (int k = 0; k < 36; ++k) total += B[k];
    return total;
}
