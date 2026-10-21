/*
 * A239250  Number of nX2 0..4 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 5.
 * https://oeis.org/A239250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A239250 – number of n×2 arrays over {0,1,2,3,4} where
   each cell cannot equal 1 plus the sum of the cells to its left
   or the sum of the cells above it (mod 5).  The computation is
   performed by a dynamic programming recurrence on the cumulative
   column sums.  The DP state is (s1,s2) where s1,s2 are the sums
   of column 1 and column 2 modulo 5 of the rows processed so far.  */
/* The recurrence is:
     dp[0][0][0] = 1
     dp[i+1][s1’,s2’] += dp[i][s1,s2] *
        count of (a1,a2) satisfying:
          a1≠1 and a1≠(1+s1) mod 5
          a2≠(1+a1) mod 5 and a2≠(1+s2) mod 5
          s1’≡s1+a1 (mod 5), s2’≡s2+a2 (mod 5)               */

#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute the transition table on first use. */
static void build_transition(seq_t transition[25][25]) {
    for (size_t ps = 0; ps < 25; ++ps) {
        size_t s1 = ps / 5;
        size_t s2 = ps % 5;
        for (size_t a1 = 0; a1 < 5; ++a1) {
            /* rule for column 1: not 1 and not 1+s1 (mod 5) */
            if (a1 == 1 || a1 == (1 + s1) % 5) continue;
            for (size_t a2 = 0; a2 < 5; ++a2) {
                /* rule for column 2: not 1+a1 (mod 5) and not 1+s2 (mod 5) */
                if (a2 == (1 + a1) % 5 || a2 == (1 + s2) % 5) continue;
                size_t ns1 = (s1 + a1) % 5;
                size_t ns2 = (s2 + a2) % 5;
                size_t ns  = ns1 * 5 + ns2;
                ++transition[ps][ns];
            }
        }
    }
}

seq_t a(idx_t n) {
    /* Ensure transition table is available */
    static bool built = false;
    static seq_t transition[25][25];
    if (!built) {
        build_transition(transition);
        built = true;
    }

    /* dp arrays for the current and next row */
    static seq_t dp[25];
    static seq_t ndp[25];

    /* initialise: before any row, sums are 0,0 */
    for (size_t i = 0; i < 25; ++i) dp[i] = 0;
    dp[0] = 1;             /* state (0,0) */

    /* iterate over rows 1 … n */
    for (idx_t row = 1; row <= n; ++row) {
        for (size_t i = 0; i < 25; ++i) ndp[i] = 0;
        for (size_t ps = 0; ps < 25; ++ps) {
            seq_t ways = dp[ps];
            if (ways == 0) continue;
            for (size_t ns = 0; ns < 25; ++ns) {
                seq_t t = transition[ps][ns];
                if (t == 0) continue;
                ndp[ns] += ways * t;
            }
        }
        for (size_t i = 0; i < 25; ++i) dp[i] = ndp[i];
    }

    /* sum over all final states */
    seq_t ans = 0;
    for (size_t i = 0; i < 25; ++i) ans += dp[i];
    return ans;
}
