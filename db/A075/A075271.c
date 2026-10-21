/*
 * A075271  a(0) = 1 and, for n >= 1, (BM)a(n) = 2*a(n-1), where BM is the
 *          BinomialMean transform.
 * https://oeis.org/A075271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(0) = 1; for n ≥ 1, Σ_{k=0}^n C(n,k)*a(k) = 2^{n+1}·a(n‑1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n) {
    static seq_t *tbl = NULL;      /* memoised values */
    static size_t cap = 0;         /* capacity of tbl   */
    static idx_t  last = 0;        /* largest index computed */

    /* Allocate table if needed */
    if (tbl == NULL) {
        cap = (size_t)(n + 1);
        tbl = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!tbl) return (seq_t)0;   /* allocation failed */
        tbl[0] = (seq_t)1;           /* a(0) */
        last = 0;
    } else if ((size_t)(n + 1) > cap) {
        cap = (size_t)(n + 1);
        tbl = (seq_t*)realloc(tbl, cap * sizeof(seq_t));
        if (!tbl) return (seq_t)0;
    }

    /* Compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                sum += binom * tbl[k];
                if (k + 1 < i) {
                    /* next binomial coefficient C(i,k+1) */
                    binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            seq_t term = pow2(i + 1) * tbl[i - 1];
            tbl[i] = term - sum;
        }
        last = n;
    }

    return tbl[n];
}
