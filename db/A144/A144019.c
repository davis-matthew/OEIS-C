/*
 * A144019  Eigentriangle read by rows, T(n,k) = A000129(n-k+1)*A006190(k).
 * https://oeis.org/A144019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A000129(n-k+1) * A006190(k) where n is the 1‑based OEIS
 *            index, k is the column position in the triangle row containing
 *            n, A000129 is the Pell sequence (1,2,5,12,29,…) and A006190
 *            (1,1,3,10,33,…) satisfies S[1]=1, S[2]=1, S[3]=3 and S[i] =
 *            3*S[i-1] + S[i-2] for i>=4
 * Range:     u8 n <= 27, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the row r for a given index n: cumulative number of elements
   up to row r is r*(r+1)/2.  Uses binary search on 128‑bit product. */
static idx_t
row_of(idx_t n)
{
    if (n <= 1)
        return 1;

    idx_t lo = 1, hi = 1;
    /* Exponential search for an upper bound */
    while (((unsigned __int128)hi * (hi + 1) / 2) < n)
        hi <<= 1;

    /* Binary search between lo and hi */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Static storage for Pell numbers P[1..] and A006190 numbers S[1..] */
static seq_t *P = NULL;          /* Pell numbers */
static seq_t *S = NULL;          /* A006190 numbers */
static idx_t  max_idx = 0;

/* Ensure that arrays P[1..need] and S[1..need] are populated. */
static void
ensure_arrays(idx_t need)
{
    if (need <= max_idx)
        return;

    /* Allocate new arrays; cast for C++. */
    seq_t *newP = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    seq_t *newS = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    if (!newP || !newS) {
        free(newP); free(newS);
        return;
    }
    /* Free old arrays if they exist */
    free(P);
    free(S);
    P = newP;
    S = newS;

    /* Initialise Pell numbers: P[1]=1, P[2]=2, P[n]=2*P[n-1]+P[n-2] */
    if (need >= 1) P[1] = (seq_t)1;
    if (need >= 2) P[2] = (seq_t)2;
    for (idx_t i = 3; i <= need; ++i)
        P[i] = P[i - 1] * (seq_t)2 + P[i - 2];

    /* Initialise A006190 numbers */
    if (need >= 1) S[1] = (seq_t)1;
    if (need >= 2) S[2] = (seq_t)1;
    if (need >= 3) S[3] = (seq_t)3;            /* base case */
    for (idx_t i = 4; i <= need; ++i)
        S[i] = S[i - 1] * (seq_t)3 + S[i - 2];

    max_idx = need;
}

/* Main entry point for the sequence. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for offset */

    /* Determine row r and column k. */
    idx_t r = row_of(n);
    idx_t prev = (r - 1) * r / 2;   /* elements before row r */
    idx_t k = n - prev;             /* column within row r, 1‑based */
    idx_t e = r - k + 1;            /* index for Pell component */

    /* Pre‑compute required values. */
    if (r > max_idx)
        ensure_arrays(r);

    return P[e] * S[k];
}
