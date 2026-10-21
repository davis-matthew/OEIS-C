/*
 * A059233  Number of rows in which n appears in Pascal's triangle A007318.
 * https://oeis.org/A059233
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = ceil(A003016(n)/2) Implementation: for each n count
 *            distinct rows r for which C(r,k)=n. k is taken only up to the
 *            middle of the row (k ≤ r/2) to avoid double counting. A small
 *            binary search is used for every k (2 ≤ k ≤ 60) to find a row
 *            with that k; the row with k=1 (C(n,1)=n) is counted i...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_leq.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* index below offset is irrelevant */

    seq_t count = 1;              /* row n via C(n,1) */

    /* maximum k that could possibly occur.
       For n up to 2^64 the bound of 60 is more than enough
       (since 4^60 >> 2^64). */
    idx_t maxk = 60;
    if (n - 1 < maxk) maxk = n - 1;

    for (idx_t k = 2; k <= maxk; ++k) {
        idx_t low = k;
        idx_t high = n;          /* r < n for k ≥ 2 */

        while (low <= high) {
            idx_t mid = low + ((high - low) >> 1);
            seq_t val = binom_leq(mid, k, n);

            if (val == n) {
                /* count this row only if k is in the first half
                   of the binomial row (to avoid duplicates). */
                if (k <= mid / 2) count++;
                break;           /* unique row for this k */
            }
            if (val < n) {
                low = mid + 1;
            } else {
                /* val > n */
                if (mid == 0) break;
                high = mid - 1;
            }
        }
    }
    return count;
}
