/*
 * A099880  Number of preferential arrangements (or simple hierarchies) of 2*n
 *          labeled elements with two kinds of elements (where each kind has n
 *          elements).
 * https://oeis.org/A099880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) * Sum_{k=0..n} k! * Stirling2(n, k)
 *            which equals binomial(2*n, n) * A000670(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for OEIS A099880. */
seq_t a(idx_t n)
{
    /* static memoization of the ordered Bell numbers A000670 */
    static size_t max_n = 0;
    static seq_t *A = NULL;
    static size_t cap   = 0;

    /* initialise memoisation table the first time */
    if (A == NULL) {
        cap = 1;
        A   = (seq_t *)malloc(cap * sizeof(seq_t));
        if (A == NULL) return 0;      /* allocation failure */
        A[0] = 1;                     /* A000670(0) = 1 */
    }

    /* grow table to contain A[n] if needed */
    if (n > max_n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            A   = tmp;
            cap = new_cap;
        }

        /* compute A[i] for i = max_n+1 .. n */
        for (size_t ii = max_n + 1; ii <= (size_t)n; ++ii) {
            idx_t  i   = (idx_t)ii;
            seq_t  sum = 0;
            seq_t  bin = 1;                /* bin = C(i, 0) = 1 */

            for (idx_t k = 0; k < i; ++k) {
                sum += bin * A[k];         /* add C(i, k) * A[k] */

                /* update bin to C(i, k+1) */
                bin = bin * (seq_t)(i - k);
                bin = bin / (seq_t)(k + 1);
            }
            A[ii] = sum;                   /* A[i] = ordered Bell number */
        }
        max_n = (size_t)n;
    }

    /* compute binomial(2*n, n) multiplicatively */
    seq_t bin = 1;                        /* bin = C(n, 0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        bin = bin * (seq_t)(n + i);
        bin = bin / (seq_t)i;
    }

    return bin * A[(size_t)n];
}
