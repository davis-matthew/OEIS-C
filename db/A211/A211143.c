/*
 * A211143  Number of 2 X 2 matrices having all terms in {-n, ..., 0, ..., n}
 *          and determinant = n^2.
 * https://oeis.org/A211143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2 × 2 matrices with entries in {-n,…,0,…,n} and
 *            determinant = n²
 * Range:     u8 n <= 5, u16 n <= 149, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* n² fits in size_t for the intended use range (n ≤ 10 000). */
    size_t n2 = (size_t)n * (size_t)n;

    /* cnt[p] = number of ordered pairs (d, m) with 1 ≤ d,m ≤ n
               and d·m = p.  uint8_t is safe: maximal count is about 81. */
    uint8_t *cnt = (uint8_t *)malloc((n2 + 1) * sizeof(uint8_t));
    if (!cnt)
        return (seq_t)0;          /* allocation failure → zero result */

    memset(cnt, 0, (n2 + 1) * sizeof(uint8_t));

    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = 1; m <= n; ++m) {
            size_t prod = (size_t)d * (size_t)m;
            cnt[prod] += 1;
        }
    }

    seq_t result = 0;
    seq_t zeroProdCount = (seq_t)(4 * n + 1);   /* bc = 0 gives 4n+1 solutions */

    for (int64_t aVal = -(int64_t)n; aVal <= (int64_t)n; ++aVal) {
        for (int64_t dVal = -(int64_t)n; dVal <= (int64_t)n; ++dVal) {
            int64_t prod = aVal * dVal;
            int64_t P     = prod - (int64_t)n * (int64_t)n;   /* ad – n² */

            if (P == 0) {
                result += zeroProdCount;
            } else {
                uint64_t absP = (P > 0) ? (uint64_t)P : (uint64_t)(-P);
                if (absP <= n2) {
                    /* 2 × number of divisor pairs for |P| that fit in the bounds */
                    result += (seq_t)(2ULL * (uint64_t)cnt[(size_t)absP]);
                }
            }
        }
    }

    free(cnt);
    return result;
}
