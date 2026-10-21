/*
 * A207381  Total sum of the odd-indexed parts of all partitions of n.
 * https://oeis.org/A207381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 92, u64 n <= 334,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A207381: Total sum of the odd‑indexed parts of all partitions of n.
   This implementation uses a dynamic programming approach that keeps
   track of the parity of the current number of parts in a partition.
   For each integer m from n down to 1 we treat m as a new part added
   to all previously constructed partitions.  The new part is appended
   after longer parts, so its row index is always `parts_before + 1`.
   If `parts_before` is even, the new part gets an odd index and contributes
   its value `m` to the running sum; otherwise it is an even index and
   contributes 0.
   Complexity: O(n²) elementary operations;  
   space: O(n) for the two parity dimensions.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* allocate two parity dimensions for part counts and odd-sum totals */
    seq_t *cnt[2], *sum[2];
    cnt[0] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    cnt[1] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    sum[0] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    sum[1] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!cnt[0] || !cnt[1] || !sum[0] || !sum[1]) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(cnt[0]); free(cnt[1]); free(sum[0]); free(sum[1]);
        return 0;
    }

    cnt[0][0] = 1;  /* empty partition – 0 parts (even) */

    for (idx_t m = n; m > 0; --m) {
        seq_t m_seq = (seq_t)m;
        for (idx_t s = m; s <= n; ++s) {
            idx_t t = s - m;
            seq_t cnt0 = cnt[0][t];
            seq_t sum0 = sum[0][t];
            seq_t cnt1 = cnt[1][t];
            seq_t sum1 = sum[1][t];

            if (cnt0 != 0) {
                /* previous parity even → new parity odd, add m to sum */
                cnt[1][s] += cnt0;
                sum[1][s] += sum0 + cnt0 * m_seq;
            }
            if (cnt1 != 0) {
                /* previous parity odd → new parity even, no addition to sum */
                cnt[0][s] += cnt1;
                sum[0][s] += sum1;
            }
        }
    }

    seq_t result = sum[0][n] + sum[1][n];

    /* clean up */
    free(cnt[0]); free(cnt[1]); free(sum[0]); free(sum[1]);

    return result;
}
