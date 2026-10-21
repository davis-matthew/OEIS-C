/*
 * A225685  Number of words of length n over {0,1,2,3,4,5} which have no
 *          factor iji with i>j.
 * https://oeis.org/A225685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n words over {0,1,2,3,4,5} that avoid
 *            the factor i j i (with i > j)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n == 0: empty word */
    if (n == 0) return (seq_t)1;

    /* n == 1: any single symbol */
    if (n == 1) return (seq_t)6;

    /* dp[p][q] = number of words of current length ending with symbols
       p (second to last) and q (last).  Alphabet size is 6 (0..5). */
    seq_t dp[6][6];
    seq_t ndp[6][6];
    for (int p = 0; p < 6; ++p)
        for (int q = 0; q < 6; ++q)
            dp[p][q] = (seq_t)1;  /* words of length 2: all pairs are allowed */

    if (n == 2) {
        seq_t total = (seq_t)0;
        for (int p = 0; p < 6; ++p)
            for (int q = 0; q < 6; ++q)
                total += dp[p][q];
        return total;  /* 36 */
    }

    for (idx_t len = 3; len <= n; ++len) {
        /* zero ndp */
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                ndp[j][k] = (seq_t)0;

        for (int p = 0; p < 6; ++p) {
            for (int q = 0; q < 6; ++q) {
                seq_t val = dp[p][q];
                if (val == (seq_t)0) continue;
                for (int k = 0; k < 6; ++k) {
                    /* forbid pattern p q p where p > q and next symbol k == p */
                    if (p > q && k == p) continue;
                    ndp[q][k] += val;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                dp[j][k] = ndp[j][k];
    }

    /* sum all entries of dp for length n */
    seq_t total = (seq_t)0;
    for (int p = 0; p < 6; ++p)
        for (int q = 0; q < 6; ++q)
            total += dp[p][q];

    return total;
}
