/*
 * A036592  Number of binary rooted trees with n nodes and height at most 9.
 * https://oeis.org/A036592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of unordered binary rooted trees with n nodes *
 *            and height at most 9 (empty tree counts as n = 0)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 34, u64 n <= 71, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_H   9
#define MAX_N  ((1U << MAX_H) - 1)    /* 511 */

/* Memoised DP table for height 9 */
static seq_t table[MAX_N + 1];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        static seq_t prev[MAX_N + 1];
        static seq_t curr[MAX_N + 1];

        /* height 0 */
        prev[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_N; ++i)
            prev[i] = (seq_t)0;

        /* Build up to height 9 */
        for (idx_t h = 1; h <= MAX_H; ++h) {
            curr[0] = (seq_t)1;   /* empty tree */

            for (idx_t nodes = 1; nodes <= MAX_N; ++nodes) {
                seq_t sum = (seq_t)0;

                /* one-child case: root has exactly one subtree of size nodes-1 */
                sum += prev[nodes - 1];

                /* two-child case: unordered pair of subtrees of sizes i and j,
                 * where i + j = nodes - 1, i >= 1, j >= 1, and i <= j.  */
                idx_t rem = nodes - 1;
                for (idx_t i = 1; i <= rem / 2; ++i) {
                    idx_t j = rem - i;          /* j >= i */
                    if (i < j) {
                        sum += prev[i] * prev[j];
                    } else { /* i == j */
                        seq_t a = prev[i];
                        /* number of unordered pairs of the same subtree type:
                           C(a + 1, 2) = a*(a+1)/2  */
                        sum += (a * (a + (seq_t)1)) / (seq_t)2;
                    }
                }

                curr[nodes] = sum;
            }

            /* copy curr to prev for next height */
            for (idx_t i = 0; i <= MAX_N; ++i)
                prev[i] = curr[i];
        }

        /* prev now holds T[9][*] */
        for (idx_t i = 0; i <= MAX_N; ++i)
            table[i] = prev[i];

        initialized = 1;
    }

    return (n > MAX_N) ? (seq_t)0 : table[n];
}
