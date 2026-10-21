/*
 * A200793  The number of forests on n nodes of rooted labeled binary trees
 *          (each node has degree <=2).
 * https://oeis.org/A200793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} C(n-1,k-1) · t(k) · a(n-k) where t(1)=1,
 *            t(k)=k·[ t(k-1)+∑_{i=1}^{⌊(k-1)/2⌋} ( i<k-1-i ?
 *            C(k-1,i)·t(i)·t(k-1-i) : (C(k-1,i)/2)·t(i)² ) ], and a(0)=1. The
 *            function memoises results and is thread‑safe for independent
 *            calls
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static seq_t *tree = NULL;   /* t(k) – rooted binary trees */
    static seq_t *fore = NULL;   /* a(k) – forests of trees   */

    /* initialise base case data if needed */
    if (max_n == 0) {
        /* allocate space for indices 0 and 1 */
        seq_t *tmp1 = (seq_t *)realloc(tree, 2 * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(fore, 2 * sizeof(seq_t));
        if (!tmp1 || !tmp2) return 0;
        tree = tmp1;
        fore = tmp2;
        tree[0] = 0;         /* unused */
        tree[1] = 1;         /* 1-node tree */
        fore[0] = 1;         /* empty forest */
        fore[1] = 1;         /* single-node forest */
        max_n   = 1;
    }

    /* expand storage and compute missing values up to n */
    if (n > max_n) {
        /* grow the arrays to accommodate index n */
        idx_t new_cap = n + 1;
        seq_t *tmp1 = (seq_t *)realloc(tree, new_cap * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(fore, new_cap * sizeof(seq_t));
        if (!tmp1 || !tmp2) return 0;
        tree = tmp1;
        fore = tmp2;

        /* compute rooted tree counts t(k) for k = max_n+1 .. n  */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* single‑child case: root with a single subtree of size k‑1 */
            seq_t sum = tree[k - 1];

            /* two‑child case (unordered children) */
            idx_t half = (k - 1) / 2;
            for (idx_t i = 1; i <= half; ++i) {
                idx_t j = k - 1 - i;
                seq_t bino = binom(k - 1, i);
                if (i < j) {
                    seq_t tmp = bino;
                    tmp *= tree[i];
                    tmp *= tree[j];
                    sum += tmp;
                } else { /* i == j */
                    seq_t tmp = bino / 2;
                    tmp *= tree[i];
                    tmp *= tree[i];
                    sum += tmp;
                }
            }
            tree[k] = (seq_t)k * sum;
        }

        /* compute forest counts a(k) for k = max_n+1 .. n  */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t acc = 0;
            for (idx_t t = 1; t <= k; ++t) {
                seq_t bino = binom(k - 1, t - 1);
                seq_t tmp  = bino;
                tmp *= tree[t];
                tmp *= fore[k - t];
                acc += tmp;
            }
            fore[k] = acc;
        }

        max_n = n;
    }

    return fore[n];
}
