/*
 * A164881  Inverse of A164844.
 * https://oeis.org/A164881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A164881: Inverse of A164844
 *
 * The sequence is built in consecutive blocks B_k (k ≥ 0).  
 * Block B_k has length k+1 and its terms are:
 *   B_k[0]          = 10^k
 *   B_k[1] (k=0)    = 1
 *   B_k[1] (k≥1)    = a pattern consisting of the digits 9,0,9,0… and finishing
 *                    by 9 or 1 (depending on the length)
 *   B_k[i] (2 ≤ i ≤ k-1) = B_{k-1}[i-1] – B_{k-1}[i]
 *   B_k[k]          = 1
 *
 * The sequence a(n) is the concatenation of the blocks:
 *   a(1) = B_0[0] = 1
 *   a(2) ← B_1[0] = 10
 *   a(3) ← B_1[1] = 1
 *   a(4) ← B_2[0] = 100
 *   a(5) ← B_2[1] = 9
 *   a(6) ← B_2[2] = 1
 *   ...
 *
 * For a given index n we locate the block in which it lies and compute
 * the necessary block values using the recurrence above.  The algorithm
 * runs in O(√n) time and uses O(√n) space.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Find the block index k such that 1 + T_k ≤ n ≤ 1 + T_k + k,
       where T_k = k(k+1)/2.  Let m = n-1; then we require T_k ≤ m < T_{k+1}. */
    idx_t m = n - 1;
    idx_t k = 0;
    idx_t T = 0;
    while (T + (k + 1) <= m) {
        T += k + 1;   /* T = T_k */
        k++;
    }
    idx_t pos = m - T;   /* 0 ≤ pos ≤ k */

    /* Generate blocks from B_0 up to B_k.  The arrays are large enough
       for all k that occur when n ≤ 10 000 (k ≤ 141). */
    static seq_t prev[512];
    static seq_t cur[512];
    idx_t prev_len = 1;
    prev[0] = (seq_t)1;   /* block B_0 */

    for (idx_t bid = 1; bid <= k; ++bid) {
        idx_t cur_len = bid + 1;
        cur[0] = seq_pow((seq_t)10, bid);   /* first element 10^k */

        if (bid == 1) {
            cur[1] = (seq_t)1;               /* B_1[1] = 1 */
        } else {
            /* Compute B_k[1] according to the digit pattern. */
            seq_t val = (seq_t)9;
            idx_t L = bid - 1;   /* number of digits in the pattern */
            for (idx_t i = 2; i <= L; ++i) {
                val = val * (seq_t)10;
                if (i % 2 == 1) val += (seq_t)9;    /* odd positions: 9 */
            }
            if ((L % 2) == 0) val += (seq_t)1;   /* even length ⇒ last digit 1 */

            cur[1] = val;

            /* Recurrence for the remaining interior terms. */
            for (idx_t p = 2; p <= bid - 1; ++p) {
                cur[p] = prev[p - 1] - prev[p];
            }
            cur[bid] = (seq_t)1;   /* last term of the block */
        }

        /* Copy the current block into prev for the next iteration. */
        for (idx_t i = 0; i < cur_len; ++i) {
            prev[i] = cur[i];
        }
        prev_len = cur_len;
    }

    return prev[pos];
}
