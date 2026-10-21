/*
 * A089975  Array read by ascending antidiagonals: T(n,k) is the number of
 *          n-letter words from a k-letter alphabet such that no letter
 *          appears more than twice.
 * https://oeis.org/A089975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(i,j) where i = s - d, j = d, s =
 *            floor((sqrt(8n+1)-1)/2), d = n - s(s+1)/2. T(0,k)=1, T(n,0)=0,
 *            T(n,k)=T(n,k-1)+n*T(n-1,k-1)+C(n,2)*T(n-2,k-1)
 * Range:     u8 n <= 48, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *dp = NULL;
static size_t dp_rows = 0, dp_cols = 0;

static void ensure_dp(size_t rows, size_t cols)
{
    if (rows <= dp_rows && cols <= dp_cols)
        return;

    /* free old table */
    free(dp);

    dp_rows = rows;
    dp_cols = cols;
    dp = (seq_t *)malloc(dp_rows * dp_cols * sizeof(seq_t));
    if (!dp) return;  /* out of memory – caller will crash with segfault */

    for (size_t n = 0; n < dp_rows; ++n) {
        for (size_t k = 0; k < dp_cols; ++k) {
            if (n == 0) {
                dp[n * dp_cols + k] = (seq_t)1;
            } else if (k == 0) {
                dp[n * dp_cols + k] = (seq_t)0;
            } else {
                seq_t term1 = dp[n * dp_cols + (k - 1)];
                seq_t term2 = (seq_t)n * dp[(n - 1) * dp_cols + (k - 1)];
                seq_t term3 = (seq_t)0;
                if (n >= 2) {
                    seq_t bin2 = ((seq_t)n * ((seq_t)n - 1)) / 2;
                    term3 = bin2 * dp[(n - 2) * dp_cols + (k - 1)];
                }
                dp[n * dp_cols + k] = term1 + term2 + term3;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* find diagonal index s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    uint64_t nn = n;              /* local copy of index */
    uint64_t s = 0, tri = 0;
    while (tri + s + 1 <= nn) {
        s++;
        tri += s;
    }
    uint64_t d = nn - tri;        /* offset on diagonal */
    uint64_t i = s - d;           /* first index (word length) */
    uint64_t j = d;               /* second index (alphabet size) */

    ensure_dp((size_t)(i + 1), (size_t)(j + 1));
    return dp[(size_t)i * dp_cols + (size_t)j];
}
