/*
 * A125363  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑26 circular n‑digit numbers with adjacent
 *            digits differing by at most 4 (defined by the adjacency matrix M
 *            with M[i][j] = 1 if |i-j| ≤ 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* static matrices to avoid repeated allocation */
    static seq_t cur[26][26];
    static seq_t nxt[26][26];
    int i, j, k;
    seq_t sum;

    /* initialize cur = identity matrix */
    for (i = 0; i < 26; ++i) {
        for (j = 0; j < 26; ++j)
            cur[i][j] = (i == j) ? 1 : 0;
    }

    /* iterate n times: cur = cur * M, where M has 1 for |i-j| ≤ 4 */
    for (k = 0; k < n; ++k) {
        /* For each row i compute prefix sums of cur[i][0..25] */
        for (i = 0; i < 26; ++i) {
            seq_t prefix[27];
            prefix[0] = 0;
            for (j = 0; j < 26; ++j)
                prefix[j + 1] = prefix[j] + cur[i][j];

            /* For each column j compute sum over neighbors of j */
            for (j = 0; j < 26; ++j) {
                int low  = (j >= 4) ? j - 4 : 0;
                int high = (j <= 21) ? j + 4 : 25;
                nxt[i][j] = prefix[high + 1] - prefix[low];
            }
        }
        /* swap cur and nxt for next iteration */
        for (i = 0; i < 26; ++i)
            for (j = 0; j < 26; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* trace of cur gives a(n) */
    sum = 0;
    for (i = 0; i < 26; ++i)
        sum += cur[i][i];
    return sum;
}
