/*
 * A034931  Triangle read by rows: Pascal's triangle (A007318) mod 4.
 * https://oeis.org/A034931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r, k) mod 4 where r = floor((sqrt(8*n + 1) - 1) / 2), k
 *            = n - r*(r + 1)/2
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r  */
    idx_t k = n - start;             /* column index (0‑based)  */

    /* Dynamic programming for binomial(r, j) mod 4 for j = 0..r */
    size_t size = (size_t)(r + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0;              /* should never happen                */

    memset(dp, 0, size * sizeof(seq_t));
    dp[0] = 1;                       /* C(r,0) = 1                         */

    for (idx_t i = 1; i <= r; ++i)
    {
        size_t limit = (i <= k) ? i : k;
        for (size_t j = limit; j >= 1; --j)
        {
            /* (dp[j-1] + dp[j]) mod 4  using bitwise AND with 3 */
            dp[j] = (dp[j - 1] + dp[j]) & 3;
        }
    }

    seq_t result = dp[k];
    free(dp);
    return result;
}
