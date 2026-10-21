/*
 * A059114  Triangle T(n,m)= Sum_{i=0..n} L'(n,i)*Product_{j=1..m} (i-j+1),
 *          read by rows.
 * https://oeis.org/A059114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,m) where r and m are determined by the row‑major
 *            ordering of the triangle T(n,k)=∑_{i=k..n} (n! /
 *            i!)·C(n‑1,i‑1)·(i)_k (falling factorial). This merely implements
 *            the definition directly
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 67, u64 n <= 173,
 *            u128 n <= 445, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t N)
{
    /* Find the row r so that r·(r+1)/2 ≤ N < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        ++r;
    idx_t col = N - r * (r + 1) / 2;   /* m = column index */

    /* Special case: the very first element */
    if (r == 0)
        return (seq_t)1;

    seq_t total = 0;

    /* Sum over i = k..n (here k = col, n = r) */
    for (idx_t i = col; i <= r; ++i)
    {
        if (i == 0)   /* L'(n,0)=0 for n>0 – skip */
            continue;

        /* L'(n,i) = (n! / i!) * C(n-1,i-1) */
        /* term = L'(n,i) * (i)_k = C(n-1,i-1) * n! / (i-k)! */
        seq_t bin = binomial(r - 1, i - 1);

        /* Compute product_{t = i-k+1}^{n} t = n! / (i-k)! */
        idx_t start = i + 1 - col;          /* i - k + 1 */
        seq_t prod  = (seq_t)1;
        for (idx_t t = start; t <= r; ++t)
            prod *= (seq_t)t;

        total += bin * prod;
    }
    return total;
}
