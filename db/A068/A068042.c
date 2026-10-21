/*
 * A068042  Number of subsets of {1,2,3,...,n} that sum to 0 mod 21.
 * https://oeis.org/A068042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS sequence A068042.
   a(n) = number of subsets of {1,2,…,n} whose element‐sum is congruent
   to 0 modulo 21.  The computation is performed by dynamic programming
   over the residues modulo 21.  For each i from 1 to n the current
   distribution of subset sums modulo 21 is updated: every previous
   residue r contributes to residue (r+i mod 21).  The result is the
   number of subsets with sum ≡ 0 (mod 21), i.e. dp[0]. */

#include <stdlib.h>

/* Compute the nth term of the sequence.  The function can be called
   with the argument n in any order; it does not maintain any
   persistent state. */
seq_t a(idx_t n)
{
    enum { MOD = 21 };            /* modulus */
    seq_t dp[MOD];
    seq_t next[MOD];

    /* initial distribution: only the empty set, sum = 0 */
    for (int r = 0; r < MOD; ++r)
        dp[r] = (seq_t)0;
    dp[0] = (seq_t)1;            /* empty subset */

    /* iterate over the elements 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        int shift = (int)(i % MOD);

        /* copy current distribution to the temporary array */
        for (int r = 0; r < MOD; ++r)
            next[r] = dp[r];

        /* add contributions from adding i to every existing subset */
        for (int r = 0; r < MOD; ++r) {
            int src = r - shift;
            if (src < 0)
                src += MOD;
            next[r] += dp[src];
        }

        /* swap the arrays: next becomes the new dp */
        for (int r = 0; r < MOD; ++r)
            dp[r] = next[r];
    }

    return dp[0];
}
