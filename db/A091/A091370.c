/*
 * A091370  Triangle read by rows: T(n,k) is the number of dissections of a
 *          convex n-gon by nonintersecting diagonals, having a k-gon over a
 *          fixed edge (base).
 * https://oeis.org/A091370
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(3)=1, a(4)=2, a(5)=1, …
   a(n) equals T(i,j) from OEIS A091370 where
   T(n,k) = ((k-1)/(n-k)) * Σ_{ℓ=0}^{n-k-1} 2^ℓ * binom(n-2, n-k-1-ℓ) * binom(n-k, ℓ),
   with T(n,n)=1. */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 3)                 /* values below the offset are undefined */
        return 0;

    /* zero‑based index within the flattened triangle */
    idx_t t = n - 3;

    /* locate the row r ≥ 3 and column k ≥ 3 corresponding to this index */
    idx_t disc = 8 * t + 1;                 /* 8*t+1 fits in idx_t for n≤10000 */
    seq_t sqrt_disc = floor_sqrt((seq_t)disc);
    idx_t m = ((idx_t)sqrt_disc - 1) / 2;    /* m = r – 3 */
    idx_t r = m + 3;                         /* row number */
    idx_t prev_total = m * (m + 1) / 2;      /* entries before row r */
    idx_t kidx = t - prev_total;             /* offset inside the row */
    idx_t k = 3 + kidx;                      /* column number */

    /* T(n,n) = 1 */
    if (k == r)
        return 1;

    /* compute the sum in the formula */
    seq_t sum = 0;
    seq_t pow2 = 1;            /* 2^0 */

    idx_t max_j = r - k - 1;
    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term = pow2;
        term = term * binom(r - 2, r - k - 1 - j);
        term = term * binom(r - k, j);
        sum += term;
        pow2 *= 2;            /* next power of two */
    }

    seq_t numerator   = (seq_t)(k - 1) * sum;
    seq_t denominator = (seq_t)(r - k);
    return numerator / denominator;
}
