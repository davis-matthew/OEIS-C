/*
 * A128718  Triangle read by rows: T(n,k) is the number of skew Dyck paths of
 *          semilength n and having k UU's (doublerises) (n >= 1; 0 <= k <=
 *          n-1).
 * https://oeis.org/A128718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major value of the triangle T(n,k) defined by
 *            T(n,0) = 1 T(n,k) = (binomial(n,k)/n) * Σ_{j=0..k} binomial(k,j)
 *            * binomial(n-k+j,j+1) for 1 ≤ k ≤ n. The index n is mapped to a
 *            row r and column k by the triangular numbers: r is the smallest
 *            row with r*(r+1)/2 ≥ n and k = n - (r-1)*r...
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline seq_t _T_value(idx_t n, idx_t k)
{
    /* T(n,0) = 1 */
    if (k == 0)
        return (seq_t)1;

    /* Compute Σ_{j=0..k} binom(k,j)*binom(n-k+j,j+1) */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t b1 = binom(k, j);
        seq_t b2 = binom(n - k + j, j + 1);
        sum += b1 * b2;
    }

    seq_t bin_n_k = binom(n, k);
    seq_t n_seq   = (seq_t)n;
    return (bin_n_k * sum) / n_seq;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n=0, safe guard */

    /* Find the row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n */
    idx_t low = 1, high = n;      /* r will never exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index k (0‑based) within that row */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri - 1;

    return _T_value(r, k);
}
