/*
 * A124858  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124858: Number of base 12 circular n‑digit numbers with adjacent digits
 * differing by 2 or less.
 *
 * Computation: a(0)=1, a(1)=12; for n≥2, maintain a 12×12 table DP where
 * DP[i][j] is the number of length‑n sequences with first digit i and last digit
 * j.  Initialise DP for n=1 as the identity matrix.  For each next length,
 *   DP_next[i][j] = Σ_{|j‑k|≤2} DP[i][k]
 * (the sum runs over all neighbours k of j in the adjacency graph of the
 * digits {0,…,11}).  After reaching the desired n, the answer is
 *
 *   a(n) = Σ_{i,j : |i‑j|≤2} DP[i][j]
 *
 * This is equivalent to a(n) = trace(Aⁿ) where A[i][j]=1 when |i‑j|≤2 and
 * 0 otherwise.
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)12;

    /* DP for length 1: identity matrix */
    seq_t dp[12][12];
    seq_t dp_next[12][12];
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            dp[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* iterate increasing length from 2 up to n */
    for (idx_t len = (idx_t)2; len <= n; ++len) {
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                seq_t sum = (seq_t)0;
                for (int k = 0; k < 12; ++k) {
                    int diff = (j >= k) ? (j - k) : (k - j);
                    if (diff <= 2) {
                        sum += dp[i][k];
                    }
                }
                dp_next[i][j] = sum;
            }
        }
        /* copy dp_next back into dp for the next iteration */
        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 12; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* compute the answer by enforcing the final adjacency constraint */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            int diff = (i >= j) ? (i - j) : (j - i);
            if (diff <= 2) {
                result += dp[i][j];
            }
        }
    }
    return result;
}
