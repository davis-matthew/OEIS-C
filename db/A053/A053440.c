/*
 * A053440  Number of k-simplices in the first derived complex of the standard
 *          triangulation of an n-simplex. Equivalently, T(n,k) is the number
 *          of ascending chains of length k+1 of nonempty subsets of the set
 *          {1, 2, ..., n+1}.
 * https://oeis.org/A053440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) is the flattened triangle value T(r,c) for OEIS A053440.
 *            T(n,k) = (k+1)! · Stirling2(n+2,k+2). Row r is given by r =
 *            ⌊(√(8n+1)−1)/2⌋. Column c = n − r(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 71, u64 n <= 181,
 *            u128 n <= 511, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_R 600   /* sufficient for n up to about 10 000 (r≈141) */

static seq_t S[MAX_R+1][MAX_R+2];   /* Stirling numbers table */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* --- compute row r and column c --- */
    idx_t t = n;                          /* t is uint64_t */
    double sd = sqrt(8.0 * (double)t + 1.0);
    idx_t r = (idx_t)((sd - 1.0) / 2.0);

    /* correct possible rounding error */
    while ((r == 0) || (r*(r+1)/2 > t))
        --r;
    while (((r+1)*(r+2)/2) <= t)
        ++r;

    /* start index of row r */
    idx_t start;
    if ((r & 1) == 0)  /* r even: r/2 * (r+1) */
        start = (r/2) * (r+1);
    else               /* r odd: r * ((r+1)/2) */
        start = r * ((r+1)/2);

    idx_t c = t - start;   /* column index in row r */

    /* --- compute factorial (c+1)! as (K-1)! where K=c+2 --- */
    idx_t K = c + 2;          /* K = k+2 */
    seq_t fact = 1;
    for (idx_t i = 2; i <= K-1; ++i)
        fact *= (seq_t)i;

    /* --- compute Stirling2(n,k) for n = r+2, k = c+2 --- */
    idx_t N = r + 2;          /* N = n+2 */

    /* ensure bounds fit into our static table */
    if (N > MAX_R)
        return 0; /* or handle overflow as needed */

    /* base case */
    S[0][0] = 1;
    for (idx_t j = 1; j <= K; ++j) S[0][j] = 0;

    /* DP compute up to N, K */
    for (idx_t i = 1; i <= N; ++i) {
        S[i][0] = 0;
        idx_t lim = (i < K) ? i : K;
        for (idx_t j = 1; j <= lim; ++j) {
            seq_t a_part = S[i-1][j-1];
            seq_t b_part = S[i-1][j] * (seq_t)j;
            S[i][j] = a_part + b_part;
        }
    }

    seq_t stir = S[N][K];
    return stir * fact;
}
