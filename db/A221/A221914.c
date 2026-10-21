/*
 * A221914  Array of products of the list entries of the nonempty combinations
 *          of n, ordered in a standard way.
 * https://oeis.org/A221914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = product of the elements of the k‑th nonempty *
 *            combination of {1,…,m} (lexicographic order, groups ordered by
 *            size), * where the index n enumerates all such products for m =
 *            1,2,3,… in * increasing m. The sequence is defined only for n ≥
 *            1
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n, idx_t k) */

seq_t a(idx_t n) {
    /* Find the row r such that the first r rows contain at least n entries. */
    idx_t r = 1;
    seq_t cumulative = 0;                     /* S(r-1)  */
    for (;;) {
        seq_t row_len = ((seq_t)1 << r) - 1;   /* 2^r – 1 */
        if (cumulative + row_len >= (seq_t)n) break;
        r++;
        cumulative += row_len;
    }

    /* Offset within the chosen row (0‑based). */
    seq_t pos = (seq_t)n - cumulative - 1;

    /* Determine the size m of the combination. */
    idx_t m = 1;
    while (1) {
        seq_t cnt = binom(r, m);              /* #subsets of size m   */
        if (cnt > pos) break;                 /* this is the group    */
        pos -= cnt;                           /* skip preceding groups */
        m++;
    }

    /* pos now ranges from 0 to binom(r,m)-1: the rank within size‑m subsets. */
    idx_t k = (idx_t)pos;                     /* index within the group */

    /* Recover the k‑th combination and compute its product. */
    seq_t prod = 1;
    idx_t prev = 0;                           /* last chosen element   */
    for (idx_t t = 0; t < m; t++) {
        for (idx_t cand = prev + 1;
             cand <= r - (m - t - 1);
             cand++) {
            seq_t remaining = binom(r - cand, m - t - 1);
            if (remaining > k) {
                prod *= (seq_t)cand;       /* include this element   */
                prev = cand;
                break;                      /* move to next position  */
            } else {
                k -= (idx_t)remaining;       /* skip this block        */
            }
        }
    }

    return prod;
}
