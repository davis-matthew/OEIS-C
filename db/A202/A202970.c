/*
 * A202970  Symmetric matrix based on A001911, by antidiagonals.
 * https://oeis.org/A202970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A202970 – Symmetric matrix based on A001911, by antidiagonals.
 *  For n ≥ 1, the n‑th term is obtained by enumerating the infinite,
 *  symmetric matrix M defined by
 *
 *      M[i][j] = Σ_{t=1}^{min(i,j)} s[t] · s[|i-j| + t],
 *      where s[k] = F(k+3) − 2 and F is the Fibonacci sequence.
 *
 *  The terms are listed along the antidiagonals starting at (1,1).
 *
 *  Complexity: O(√n) for a call with index n (d ~ √(2n)),
 *  which is well below the time limit for n up to 10 000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                   /* sequence is 1‑based; guard */
        return 0;

    /* 1. Locate the antidiagonal and offset within it. */
    idx_t d = 0;                   /* diagonal index   (i + j - 2) */
    idx_t cum = 0;                 /* elements before diagonal d */
    /* Find smallest d with cum + (d + 1) >= n */
    while ((cum + (d + 1)) < n) {
        cum += ++d;                /* cumulative += d+1 */
    }
    idx_t k = n - cum - 1;         /* offset along the diagonal, 0‑based */

    /* 2. Convert (d, k) to matrix coordinates (i, j). */
    idx_t i = k + 1;
    idx_t j = d + 1 - k;           /* because diagonal has d+1 elements */

    /* 3. Prepare parameters for the sum. */
    idx_t dabs = (i > j) ? (i - j) : (j - i);
    idx_t m    = (i < j) ? i : j;  /* min(i, j) */

    /* 4. Compute the required Fibonacci numbers up to index
     *    m + dabs + 3 (since s[t] = F(t+3)-2). */
    idx_t fib_need = m + dabs + 3;          /* highest Fibonacci index needed */
    seq_t *fib = (seq_t*)malloc((fib_need + 1) * sizeof(seq_t));
    if (!fib)
        exit(1);   /* minimal error handling */

    fib[0] = 0;
    fib[1] = 1;
    for (idx_t kf = 2; kf <= fib_need; ++kf)
        fib[kf] = fib[kf - 1] + fib[kf - 2];

    /* 5. Evaluate the matrix entry. */
    seq_t result = 0;
    for (idx_t t = 1; t <= m; ++t) {
        seq_t s1 = fib[t + 3] - 2;            /* s[t]   */
        seq_t s2 = fib[dabs + t + 3] - 2;     /* s[t+d] */
        result += s1 * s2;
    }

    free(fib);
    return result;
}
