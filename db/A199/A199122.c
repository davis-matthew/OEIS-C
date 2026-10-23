/*
 * A199122  Number of partitions of n into terms of (2,3)-Ulam sequence, cf.
 *          A001857.
 * https://oeis.org/A199122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A199122
 *  Number of partitions of n into terms of the (2,3)-Ulam sequence (A001857).
 *  The Ulam sequence is defined with first terms 2,3 and subsequent terms are
 *  the smallest integer larger than the previous one that can be expressed
 *  uniquely as the sum of two *distinct* earlier terms.
 *  The function computes the number of unordered partitions of n whose parts
 *  belong to this Ulam sequence.
 *  Complexity: O(k·n) where k = number of Ulam terms ≤ n  (≈ O(n)).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  ----  Generate Ulam(2,3) terms up to n  ----  */
    /* allocate array for terms – at most n+1 terms (very generous) */
    idx_t *ulam = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (ulam == NULL) return 0;                /* allocation failure */

    idx_t ulen = 0;                            /* current number of terms   */

    /* first terms: 2, 3 if they are ≤ n */
    if (n >= 2) ulam[ulen++] = 2;
    if (n >= 3) ulam[ulen++] = 3;

    /* array for counting representations of sums up to n
     * we only need to detect sums that could become future terms ≤ n
     */
    unsigned short *scount = (unsigned short *)
                             calloc((size_t)n + 1, sizeof(unsigned short));
    if (scount == NULL) { free(ulam); return 0; }

    /* initialise: 2+3 = 5 */
    if (ulen >= 2) {
        idx_t s = ulam[0] + ulam[1];
        if (s <= n) scount[s] = 1;
    }

    /*  generate all Ulam terms up to n   */
    for (idx_t v = 4; v <= n;) {          /* candidate value */
        /* look for the next Ulam term */
        for (; v <= n && scount[v] != 1; ++v);
        if (v > n) break;                /* no more terms within limit */

        /* add the new term */
        ulam[ulen++] = v;

        /* update sums involving the new term and each earlier term
         * only distinct pairs, so j < current index
         */
        for (idx_t j = 0; j + 1 < ulen; ++j) {
            idx_t s = ulam[j] + v;
            if (s <= n) {
                /* safety wrap‑around for unsigned short: it never overflows
                 * here because the count of representations for any sum ≤ n
                 * is tiny (strictly < 2⁶⁵ in practice).
                 */
                scount[s] = (unsigned short)(scount[s] + 1);
            }
        }
        ++v;    /* continue search with next possible candidate */
    }

    /*  ----  Partition DP  ----  */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL) {
        free(ulam);
        free(scount);
        return 0;
    }

    dp[0] = 1;          /* empty partition */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    for (idx_t i = 0; i < ulen; ++i) {
        idx_t term = ulam[i];
        for (idx_t j = term; j <= n; ++j) {
            dp[j] = (seq_t)((seq_t)dp[j] + (seq_t)dp[j - term]);
        }
    }

    /*  ----  Cleanup & return  ----  */
    seq_t result = dp[n];
    free(dp);
    free(ulam);
    free(scount);
    return result;
}
