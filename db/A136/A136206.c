/*
 * A136206  Triangle H(n,j) (n=1,2,3,..., j=2,3,4,...) read by rows: let
 *          X(k,l,n) := Stirling2(n,k)*Stirling2(k,l) for 1<=k<=n and 1<=l<=k.
 *          Then H(n,j)= Sum_{k+l=j, 1<=k<=n and 1<=l<=k} X(k,l,n).
 * https://oeis.org/A136206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   The OEIS term a(n) is the entry H(r, j) of the triangle defined
 *            by H(n, j) = sum_{k + l = j} Stirling2(n, k) * Stirling2(k, l)
 *            where Stirling2(m, t) denotes the Stirling numbers of the second
 *            kind. The flattening of the triangle read row‑wise gives the
 *            sequence. The index n is mapped to a row r a...
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* high exclusive */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = ( __uint128_t )mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Table of Stirling numbers S[n][k].  S[n][k] = Stirling2(n, k).
   The table is expanded only up to the largest row requested.  */
static seq_t **S = NULL;
static idx_t S_max = 0;   /* largest n for which S[n] is allocated */

/* Expand the Stirling table up to row n (inclusive). */
static void ensure_S(idx_t n)
{
    if (S == NULL) {
        /* allocate array of pointers for 0..n */
        S = (seq_t**)malloc((n + 1) * sizeof(seq_t*));
        /* base case S[0][0] = 1 */
        S[0] = (seq_t*)malloc(sizeof(seq_t));
        S[0][0] = (seq_t)1;
        S_max = 0;
        if (n == 0) return;
    }

    /* Grow the pointer array if necessary */
    if (n > S_max) {
        S = (seq_t**)realloc(S, (n + 1) * sizeof(seq_t*));
    }

    /* Compute Stirling numbers for rows S_max+1 .. n */
    for (idx_t row = S_max + 1; row <= n; ++row) {
        S[row] = (seq_t*)malloc((row + 1) * sizeof(seq_t));
        S[row][0] = (seq_t)0;           /* Stirling2(row, 0) = 0 for row > 0 */
        for (idx_t k = 1; k <= row; ++k) {
            seq_t val = S[row - 1][k - 1];
            if (k <= row - 1) {
                val += (seq_t)k * S[row - 1][k];
            }
            S[row][k] = val;
        }
    }
    S_max = n;
}

/* Main routine required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n = 0, return 0 */

    /* Determine row and column (j) of the requested term */
    idx_t base = floor_sqrt_u64(n);
    idx_t row = ( ( (__uint128_t)base * base ) == n ) ? base : base + 1;

    /* Position within the row (0‑based) */
    __uint128_t prev_sq128 = ( __uint128_t )(row - 1) * (row - 1);
    idx_t prev_sq = (idx_t)prev_sq128;
    idx_t pos = n - prev_sq - 1;

    idx_t j = 2 + pos;              /* j ranges from 2 to 2*row */

    /* Ensure Stirling numbers up to this row are available */
    ensure_S(row);

    /* Compute H(row, j) */
    seq_t result = (seq_t)0;
    idx_t k_min = (j + 1) / 2;      /* ceil(j/2) */
    if (k_min < 1) k_min = 1;
    idx_t k_max = (j - 1 < row) ? (j - 1) : row;

    for (idx_t k = k_min; k <= k_max; ++k) {
        seq_t prod = S[row][k] * S[k][j - k];
        result += prod;
    }
    return result;
}
