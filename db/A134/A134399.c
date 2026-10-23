/*
 * A134399  Matrix product of Binomial triangle A007318 and triangle with (1,
 *          1, 2, 3, 4, 5, ...) in the main diagonal and the rest zeros.
 * https://oeis.org/A134399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th entry in the row‑major flattening of the
   matrix product of the binomial coefficients T(n,k)=C(n,k) and a
   diagonal matrix D with D(k,k)=1 for k=0,1 and D(k,k)=k for k>=2.
   For index n, find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2,
   then the column k = n - r(r+1)/2. Finally a(n) = C(r,k) * d_k where
   d_k = 1 if k<=1 else k. */

#include "../utils/binom.h"

static idx_t row_of(idx_t n)
{
    /* Find largest r with r*(r+1)/2 <= n.  Binary search uses 128‑bit
       arithmetic to avoid overflow. */
    idx_t low = 0;
    idx_t high = 1;
    while (1) {
        unsigned __int128 val = (unsigned __int128)high * (high + 1) / 2;
        if (val > n) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k that correspond to index n. */
    idx_t r = row_of(n);
    idx_t prev = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - prev;                 /* 0‑based column index in row r */

    /* Diagonal factor: 1 for k=0,1 and k otherwise. */
    seq_t d = (k <= 1) ? (seq_t)1 : (seq_t)k;

    /* Binomial coefficient C(r,k) multiplied by the diagonal factor. */
    seq_t bin = binom(r, k);
    return bin * d;
}
