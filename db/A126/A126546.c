/*
 * A126546  Number of base 25 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   f[1][d] = 1   for d = 0..24
   f[k+1][d] = sum_{|d - d'| <= 5} f[k][d']
   a(n) = sum_{d=0}^{24} f[n][d],  a(0) = 1
   (n=0 represents the empty string) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t dp[25];
    seq_t next[25];

    /* initial count for length 1: all digits are allowed */
    for (idx_t i = 0; i < 25; ++i)
        dp[i] = (seq_t)1;

    /* iterate lengths from 2 up to n */
    for (idx_t len = 1; len < n; ++len) {
        for (idx_t d = 0; d < 25; ++d) {
            seq_t sum = 0;
            idx_t low  = (d > 5) ? d - 5 : 0;
            idx_t high = (d + 5 < 25) ? d + 5 : 24;
            for (idx_t k = low; k <= high; ++k)
                sum += dp[k];
            next[d] = sum;
        }
        for (idx_t d = 0; d < 25; ++d)
            dp[d] = next[d];
    }

    /* total number of length-n strings */
    seq_t total = 0;
    for (idx_t d = 0; d < 25; ++d)
        total += dp[d];

    return total;
}
