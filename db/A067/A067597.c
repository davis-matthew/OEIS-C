/*
 * A067597  Number of partitions of n into distinct palindrome parts in base
 *          2.
 * https://oeis.org/A067597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of partitions of n into distinct binary palindrome
 *            parts
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all binary palindromes <= n, then counts the subsets
   that sum to n using a standard 1‑D DP that respects distinctness.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Count partitions of n into distinct binary palindromes. */
seq_t a(idx_t n)
{
    /* Base case: only the empty partition for n = 0. */
    if (n == 0) return 1;

    /* Generate all binary palindromes <= n.  For n <= 10000 there are <= 256 such numbers. */
    seq_t pal[512];
    size_t pal_cnt = 0;

    /* Determine maximum length in bits needed. */
    int max_len = 0;
    idx_t tmp = n;
    while (tmp) { ++max_len; tmp >>= 1; }

    for (int len = 1; len <= max_len; ++len) {
        int half = (len + 1) / 2;
        idx_t start = (idx_t)1 << (half - 1);      /* leading bit must be 1 */
        idx_t end   = (idx_t)1 << half;            /* exclusive upper bound */

        for (idx_t hv = start; hv < end; ++hv) {
            /* Build the full palindrome from the 'half' value. */
            seq_t pal_val = (seq_t)hv;
            seq_t rev = hv >> (len & 1);            /* skip middle bit for odd length */

            for (int i = 0; i < len / 2; ++i) {
                pal_val = (pal_val << 1) | (rev & 1);
                rev >>= 1;
            }

            if (pal_val <= n) {
                pal[pal_cnt++] = pal_val;
            }
        }
    }

    /* DP array for counting subsets.  dp[i] = number of ways to sum to i. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure → return 0 (should not happen) */
    dp[0] = 1;                   /* empty subset sums to 0 */

    /* Process each palindrome exactly once (distinct parts). */
    for (size_t k = 0; k < pal_cnt; ++k) {
        seq_t p = pal[k];
        idx_t p_idx = (idx_t)p;        /* safe cast: p <= n */
        for (idx_t i = n; i >= p_idx; --i) {
            dp[i] += dp[i - p_idx];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
