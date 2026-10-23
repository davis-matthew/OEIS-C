/*
 * A193700  Number of arrays of -6..6 integers x(1..n) with every x(i) in a
 *          subsequence of length 1, 2 or 3 with sum zero.
 * https://oeis.org/A193700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193700: number of arrays of integers -6..6 (x[1..n]) such that
   every x[i] lies in some contiguous subsequence of length 1, 2 or 3 with sum zero.
   Computation is performed by a dynamic programming over the last two values
   with flags indicating whether those values are already satisfied. The DP
   state after processing k (k >= 2) elements is:
        (v_{k-1}, v_k, sat_{k-1}, sat_k)
   where sat_j is true if position j has already been satisfied by some
   zero-sum window contained in the processed prefix.  After appending a
   new value v_{k+1}, the element v_{k-1} becomes final and must be
   satisfied.  The transitions below implement this rule.  The base case
   for n=1 is handled separately.  The algorithm runs in O(n) time
   with a constant 13^3*4 ≈ 8800 state transitions per step.  */

/* OFF maps the integer value -6..6 to indices 0..12 */
#define OFF 6

seq_t a(idx_t n)
{
    /* n = 0 is outside the defined range; return 0 just in case */
    if (n == 0) return 0;

    /* n = 1: only array [0] satisfies the condition */
    if (n == 1) return 1;

    /* DP arrays: dp[i1][i2][s1][s2] where i1,i2 are indices of values a[pos-2],a[pos-1] */
    static seq_t dp[13][13][2][2];
    static seq_t nxt[13][13][2][2];
    /* clear dp */
    for (int i1 = 0; i1 < 13; ++i1)
        for (int i2 = 0; i2 < 13; ++i2)
            for (int s1 = 0; s1 < 2; ++s1)
                for (int s2 = 0; s2 < 2; ++s2)
                    dp[i1][i2][s1][s2] = 0;

    /* Initialisation for the first two elements */
    for (int i1 = 0; i1 < 13; ++i1) {
        int v1 = i1 - OFF;
        int sat1_initial = (v1 == 0);        /* single element satisfaction */
        for (int i2 = 0; i2 < 13; ++i2) {
            int v2 = i2 - OFF;
            int sat1 = sat1_initial || (v1 + v2 == 0);  /* pair may satisfy first */
            int sat2 = (v2 == 0) || (v1 + v2 == 0);      /* pair may satisfy second */
            dp[i1][i2][sat1][sat2] += 1;
        }
    }

    /* If the required length is 2 we are done here; otherwise loop for positions 3..n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        /* clear next DP */
        for (int i1 = 0; i1 < 13; ++i1)
            for (int i2 = 0; i2 < 13; ++i2)
                for (int s1 = 0; s1 < 2; ++s1)
                    for (int s2 = 0; s2 < 2; ++s2)
                        nxt[i1][i2][s1][s2] = 0;

        for (int i1 = 0; i1 < 13; ++i1) {
            int v1 = i1 - OFF;            /* a[pos-2] */
            for (int i2 = 0; i2 < 13; ++i2) {
                int v2 = i2 - OFF;        /* a[pos-1] */
                for (int s1 = 0; s1 < 2; ++s1) {
                    for (int s2 = 0; s2 < 2; ++s2) {
                        seq_t c = dp[i1][i2][s1][s2];
                        if (c == 0) continue;
                        int sum12 = v1 + v2;            /* a[pos-2] + a[pos-1] */
                        for (int i3 = 0; i3 < 13; ++i3) {
                            int v3 = i3 - OFF;          /* a[pos]   */
                            int sum123 = sum12 + v3;    /* full triple */

                            int sat_first_final = s1 || (sum123 == 0);
                            if (!sat_first_final) continue;   /* first element remains unsatisfied -> drop this path */

                            int sat_second_new = s2 || (v2 + v3 == 0) || (sum123 == 0);
                            int sat_new        = (v3 == 0) || (v2 + v3 == 0) || (sum123 == 0);

                            nxt[i2][i3][sat_second_new][sat_new] += c;
                        }
                    }
                }
            }
        }

        /* copy nxt into dp for the next iteration */
        for (int i1 = 0; i1 < 13; ++i1)
            for (int i2 = 0; i2 < 13; ++i2)
                for (int s1 = 0; s1 < 2; ++s1)
                    for (int s2 = 0; s2 < 2; ++s2)
                        dp[i1][i2][s1][s2] = nxt[i1][i2][s1][s2];
    }

    /* Sum all states where the last two elements are satisfied */
    seq_t total = 0;
    for (int i1 = 0; i1 < 13; ++i1)
        for (int i2 = 0; i2 < 13; ++i2)
            total += dp[i1][i2][1][1];

    return total;
}
