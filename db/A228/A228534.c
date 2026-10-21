/*
 * A228534  Triangular array read by rows: T(n,k) is the number of functional
 *          digraphs on {1,2,...,n} such that every element is mapped to a
 *          recurrent element and there are exactly k cycles, n>=1, 1<=k<=n.
 * https://oeis.org/A228534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of functional digraphs on {1..n} such that every
 *            element is mapped to a recurrent element and there are exactly k
 *            cycles for the k corresponding to n in the triangular array;
 *            a(n) is the row‑by‑row flattening of that array
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 56, u64 n <= 154,
 *            u128 n <= 436, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Forward declaration of helper function computing T(n,k) */
static seq_t triangular_T(idx_t n, idx_t k);

/* Main entry point as required */
seq_t a(idx_t n)
{
    /* Locate the row (n) and column (k) in the triangular array */
    /* Row r satisfies r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t r = (idx_t)((sqrt(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    if (r * (r + 1) / 2 < n) r++;                     /* round up if needed */
    idx_t prev = (r - 1) * r / 2;                      /* elements before this row */
    idx_t k = n - prev;                                /* 1‑based column index */

    return triangular_T(r, k);
}

/* Computes T(n,k) = number of functional digraphs on n labeled vertices
   satisfying the properties and having exactly k cycles. */
static seq_t triangular_T(idx_t n, idx_t k)
{
    /* Allocate array for Stirling numbers c(s,j) up to current k.
       cur[j] holds c(current_s, j). */
    size_t sz = (size_t)k + 1;
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur) return (seq_t)0;           /* safety, should not happen */

    cur[0] = (seq_t)1;                  /* c(0,0) = 1 */

    seq_t binom = (seq_t)1;              /* C(n,0) = 1 */
    seq_t sum   = (seq_t)0;              /* accumulating T(n,k) */

    for (idx_t s = 0; s <= n; ++s) {
        if (s >= k) {
            /* term = C(n,s) * c(s,k) * s^(n-s) */
            seq_t c_sk = cur[k];              /* c(s,k) */
            seq_t pow_s = big_pow((seq_t)s, (seq_t)(n - s));
            seq_t term  = binom * c_sk;
            term       *= pow_s;
            sum        += term;
        }

        /* Prepare values for next s + 1 */
        if (s == n) break;                   /* last iteration done */

        /* Update Stirling numbers for next s (s+1) */
        /* cur[j] = cur[j-1] + (s) * cur[j]  for j = min(s+1,k) ... 1 */
        size_t jmax = (s + 1 <= k) ? (size_t)(s + 1) : sz - 1;
        for (size_t j = jmax; j >= 1; --j)
            cur[j] = cur[j - 1] + cur[j] * (seq_t)s;
        cur[0] = (seq_t)0;

        /* Update binomial coefficient C(n, s+1) from C(n, s) */
        binom = binom * (seq_t)(n - s) / (seq_t)(s + 1);
    }

    free(cur);
    return sum;
}
