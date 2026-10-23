/*
 * A169377  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            * on 28 generators with relations (S_i)^2 = (S_i S_j)^31 = I. *
 *            Computed by dynamic programming counting all length‑n words
 *            avoiding * two forbidden patterns: (i,i) and any length‑31
 *            alternating pattern * between two distinct letters...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* dp0: words ending with a run of length 1 (last letter is distinct
	   from the previous one and from the one before that, if any)      */
    seq_t dp0 = (seq_t)28;      /* length 1: any of the 28 letters */
    seq_t dp1[31];              /* dp1[k] : words of current length ending
				   with an alternating run of length k (k>=2)  */
    for (int i = 0; i <= 30; ++i) dp1[i] = (seq_t)0;

    /* Iterate word length from 2 up to n */
    for (idx_t len = 2; len <= n; ++len)
    {
        seq_t new_dp0 = (seq_t)0;
        seq_t new_dp1[31];
        for (int i = 0; i <= 30; ++i) new_dp1[i] = (seq_t)0;

        /* Transition from run of length 1 : pick any of 27 other letters
           to start an alternating run of length 2. */
        new_dp1[2] += dp0 * (seq_t)27;

        /* Transition from existing alternating runs */
        for (int k = 2; k <= 30; ++k)
        {
            seq_t val = dp1[k];
            if (val == (seq_t)0) continue;

            /* Continue alternation if k < 30 (next letter = the other letter) */
            if (k < 30)
                new_dp1[k + 1] += val;

            /* Break the run: choose any of the 26 letters not used in the run
               to start a new run of length 1. */
            new_dp0 += val * (seq_t)26;
        }

        /* Advance to next length */
        dp0 = new_dp0;
        for (int i = 2; i <= 30; ++i) dp1[i] = new_dp1[i];
    }

    /* Sum all states to obtain a(n) */
    seq_t total = dp0;
    for (int i = 2; i <= 30; ++i) total += dp1[i];
    return total;
}
