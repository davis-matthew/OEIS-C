/*
 * A050159  T(n,k) = S(2n-1,n-1,k-1), 0<=k<=n, n >= 0, array S as in A050157.
 * https://oeis.org/A050159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened square‑triangular array T(r, k) (row r,
 *            column k) defined by T(r, 0) = Catalan(r‑1) (r ≥ 1, with
 *            Catalan(–1)=0 for r=0) T(r, k>0) = binom(2r–1, r–1) –
 *            binom(2r–1, r–1–k) The index n of a(n) is obtained by forming
 *            rows of lengths r+1 for r = 0,1,2,…; that is, the first
 *            (r+1)(...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Special case: the very first element (row 0) is 0. */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that the first element of row r has index
       cum = r(r+1)/2 and the first element of row r+1 has index
       cum + (r+1).  Incrementally find r; for n ≤ 10 000 this loop
       finishes in a few dozen iterations. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (n >= cum + r + 1) {
        cum += r + 1;
        r++;
    }

    /* Column k within the found row. */
    idx_t k = n - cum;

    /* Evaluate T(r, k). */
    if (k == 0) {
        /* T(r,0) = Catalan(r-1);  for r=1, Catalan(0)=1. */
        return catalan(r - 1);
    } else {
        /* T(r,k) = binom(2r-1, r-1) – binom(2r-1, r-1-k). */
        seq_t base = binom(2 * r - 1, r - 1);
        seq_t sub  = 0;
        if (k < r)            /* otherwise r-1-k would underflow */
            sub = binom(2 * r - 1, r - 1 - k);
        return base - sub;
    }
}
