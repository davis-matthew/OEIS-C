/*
 * A096647  Number of partitions of an n-set with even number of even blocks.
 * https://oeis.org/A096647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n, (n‑k) even} S(n,k) (S(n,k) = Stirling numbers
 *            of the second kind)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *A      = NULL;   /* values a[0..m] */
    static size_t capA   = 0;      /* A size: maximal computed index + 1 */
    static seq_t *prev   = NULL;   /* Stirling row for n‑1: S(n‑1,0..n‑1) */
    static size_t prev_len = 0;     /* length of prev (n‑1 + 1) */

    /* --- initialise for n = 0 ------------------------------------- */
    if (capA == 0) {
        /* a(0) = 1  (only the empty partition) */
        A = (seq_t*)malloc(sizeof(seq_t));
        A[0] = (seq_t)1;
        capA = 1;

        /* S(0,0) = 1 */
        prev = (seq_t*)malloc(sizeof(seq_t));
        prev[0] = (seq_t)1;
        prev_len = 1;
    }

    /* --- compute further rows up to n if necessary --------------- */
    for (idx_t m = capA; m <= n; ++m) {
        /* Compute Stirling row for n = m  */
        size_t new_len = (size_t)m + 1;          /* S(m,0..m) */
        seq_t *cur = (seq_t*)malloc(new_len * sizeof(seq_t));

        /* S(m,0) = 0, S(m,m) = 1,  S(m,k) = k*S(m‑1,k) + S(m‑1,k‑1) */
        for (size_t k = 0; k < new_len; ++k) {
            if (k == 0) {
                cur[k] = (seq_t)0;
            } else if (k == new_len - 1) {
                cur[k] = (seq_t)1;
            } else {
                /* cur[k] = k * prev[k] + prev[k‑1]  (k < prev_len) */
                cur[k] = prev[k] * (seq_t)k + prev[k - 1];
            }
        }

        /* Sum the terms with (m‑k) even  <=> (k and m have same parity) */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < new_len; ++k) {
            if ( ((k & 1) == (m & 1)) ) {
                sum += cur[k];
            }
        }

        /* Resize A if necessary and store a[m] */
        if (capA <= m) {
            A = (seq_t*)realloc(A, (m + 1) * sizeof(seq_t));
        }
        A[m] = sum;

        /* prepare for next iteration */
        free(prev);
        prev = cur;
        prev_len = new_len;
        capA = m + 1;
    }

    return A[n];
}
