/*
 * A200838  T(n,k)=Number of 0..k arrays x(0..n+1) of n+2 elements without any
 *          two consecutive increases or two consecutive decreases.
 * https://oeis.org/A200838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 129, u64 n <= 396,
 *            u128 n <= 1208, big any n
 * Helpers:   sign_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200838
   a(n) = T(n1, k1) where (n1,k1) are obtained from n by
   antidiagonal enumeration of the table T(i,j):
        T(i,j) counts 0..j arrays of length i+2 without any two
        consecutive increases or two consecutive decreases.
   The table is read by increasing sums i+j, with i starting at 1.
   Thus the mapping from n to (i,j) is:
        let s be the smallest integer with n <= s*(s-1)/2
        let prev = (s-2)*(s-1)/2   // total entries before diagonal s
        offset = n - prev - 1      // 0‑based index within diagonal
        i = offset + 1
        j = s - i
   We then compute T(i,j) by dynamic programming. */

#include <stdlib.h>
#include <string.h>
#include "../utils/sign_idx.h"

seq_t a(idx_t n)
{
    /* Find diagonal s such that n <= s*(s-1)/2 */
    idx_t s = 2;
    while (n > s * (s - 1) / 2)
        ++s;

    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev - 1;          /* 0‑based within diagonal */
    idx_t i = offset + 1;                 /* row index in the table */
    idx_t j = s - i;                      /* column index in the table */
    /* i and j are at least 1 */

    /* Length of arrays to consider: L = i + 2 */
    idx_t L = i + 2;                      /* L >= 3 */

    /* DP arrays: states (value, sign).  Sign indices: 0=-1, 1=0, 2=+1 */
    size_t k = (size_t)j;
    size_t bufsz = (k + 1) * 3;

    seq_t *dp_cur = (seq_t *)malloc(bufsz * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(bufsz * sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* Handle allocation failure: return 0 as a safe fallback */
        free(dp_cur);
        free(dp_next);
        return (seq_t)0;
    }
    memset(dp_cur, 0, bufsz * sizeof(seq_t));
    memset(dp_next, 0, bufsz * sizeof(seq_t));

    /* Base: all length-2 sequences */
    for (idx_t a = 0; a <= j; ++a) {
        for (idx_t b = 0; b <= j; ++b) {
            int diff = (int)b - (int)a;
            int sIdx = sign_idx(diff);
            /* dp_cur index: value b, sign sIdx */
            dp_cur[b * 3 + sIdx] += (seq_t)1;
        }
    }

    /* Extend to length L */
    for (idx_t pos = 2; pos < L; ++pos) {
        /* reset next table */
        memset(dp_next, 0, bufsz * sizeof(seq_t));

        for (idx_t last = 0; last <= j; ++last) {
            for (int sIdx = 0; sIdx < 3; ++sIdx) {
                seq_t ways = dp_cur[last * 3 + sIdx];
                if (!ways)
                    continue;
                for (idx_t w = 0; w <= j; ++w) {
                    int diff = (int)w - (int)last;
                    int s2Idx = sign_idx(diff);
                    /* Skip two consecutive strict increases or decreases */
                    if ((sIdx == 0 && s2Idx == 0) || (sIdx == 2 && s2Idx == 2))
                        continue;
                    dp_next[w * 3 + s2Idx] += ways;
                }
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all final states */
    seq_t total = (seq_t)0;
    for (idx_t v = 0; v <= j; ++v)
        for (int sIdx = 0; sIdx < 3; ++sIdx)
            total += dp_cur[v * 3 + sIdx];

    free(dp_cur);
    free(dp_next);
    return total;
}
