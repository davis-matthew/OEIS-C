/*
 * A197505  Exponential transform of A006125.
 * https://oeis.org/A197505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} binom(n-1,k-1) · 2^{k(k-1)/2} · a(n‑k) a(0) =
 *            1 , a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *A = NULL;   /* A[i] holds a(i) */
    static idx_t cap   = 0;   /* allocated size of A          */
    static idx_t done  = 0;   /* number of elements already
                                 computed (indices 0 .. done-1) */

    /* enlarge allocation if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        A = tmp;
        /* initialise new entries with zero */
        for (idx_t i = cap; i < new_cap; ++i) A[i] = 0;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if (n + 1 > done) {
        for (idx_t i = done; i <= n; ++i) {
            if (i == 0) { A[0] = 1; break; }
            if (i == 1) { A[1] = 1; break; }

            seq_t result = 0;
            seq_t binom  = 1;         /* binom(n-1, k-1) for current k */
            seq_t c      = 1;         /* 2^{k(k-1)/2} for current k (k=1) */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = binom * 2^{k(k-1)/2} * a(i-k) */
                seq_t term = binom * c * A[i - k];
                result += term;

                if (k < i) {
                    /* update binomial coefficient:
                       binom(n-1, k) = binom(n-1, k-1) * (n-k) / k */
                    binom = (binom * (seq_t)(i - k)) / (seq_t)k;

                    /* update coefficient: shift by k bits
                       2^{(k+1)k/2} = 2^{k(k-1)/2} * 2^{k} */
                    c <<= k;
                }
            }
            A[i] = result;
        }
        done = n + 1;
    }

    return A[n];
}
