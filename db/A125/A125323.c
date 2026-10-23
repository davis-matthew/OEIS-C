/*
 * A125323  Number of base 13 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 13×13 adjacency matrix with *
 *            A[i][j] = 1 iff |i - j| ≤ 3 (including 0). a(0) = 1. * The
 *            function keeps a running power of A and memoises the traces so *
 *            that each new value can be obtained in O(1) additional work
 *            after * an O(n) initialisation. Only 64-bit arithm...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static working state */
    static int cur      = 0;
    static int nxt      = 1;
    static seq_t matrix[2][13][13];
    static size_t   max_n = 0;
    static size_t   cap   = 0;
    static seq_t *trace = NULL;

    const int D = 13;   /* number of digits 0‑12 */

    /* initialise on first invocation */
    if (trace == NULL) {
        cap = 1;
        trace = (seq_t *)malloc(cap * sizeof(seq_t));
        trace[0] = (seq_t)1;                 /* a(0) = 1 */

        /* identity matrix: A^0 */
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                matrix[cur][i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    if (n <= max_n)           /* already computed */
        return trace[n];

    /* grow the trace array if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        trace = (seq_t *)realloc(trace, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* iteratively multiply by the 0/1 adjacency matrix */
    for (size_t step = max_n + 1; step <= (size_t)n; ++step) {
        /* zero the next matrix */
        for (int i = 0; i < D; ++i)
            for (int j = 0; j < D; ++j)
                matrix[nxt][i][j] = (seq_t)0;

        /* matrix multiplication by A (entries are 0/1) */
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                seq_t val = matrix[cur][i][j];
                if (val == (seq_t)0) continue;
                int start = (j > 3) ? (j - 3) : 0;
                int end   = (j + 3 < 12) ? (j + 3) : 12;
                for (int k = start; k <= end; ++k)
                    matrix[nxt][i][k] += val;
            }
        }

        /* compute trace of the new matrix */
        seq_t t = (seq_t)0;
        for (int i = 0; i < D; ++i)
            t += matrix[nxt][i][i];
        trace[step] = t;

        /* advance to next power */
        int temp = cur;
        cur = nxt;
        nxt = temp;
    }

    max_n = n;
    return trace[n];
}
