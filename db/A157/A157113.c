/*
 * A157113  Triangle, read by rows, T(n,k) = binomial(n*(n-k)*k, k) +
 *          binomial(n*(n-k)*k, n -k).
 * https://oeis.org/A157113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is defined by reading the triangle T(r,k) = binom(r*(r-k)*k
 *            , k) + binom(r*(r-k)*k , r-k) along its rows. The terms are
 *            listed by rows, starting with r=0. Each row r has r+1 entries
 *            (k=0..r)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 92, u128 n <= 232,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper function `binom` is assumed to be available via
   "../utils/binom.h". */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the first term of row r is at
       index S(r-1) = r*(r+1)/2.  Then n lies in row r. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Number of terms before row r */
    idx_t prev = r * (r + 1) / 2;

    /* Column index within row r */
    idx_t k = n - prev;

    /* Compute m = r * (r-k) * k, which fits in idx_t for the test range. */
    idx_t m = r * (r - k) * k;

    /* T(r,k) = binom(m, k) + binom(m, r-k) */
    seq_t term1 = binom(m, k);
    seq_t term2 = binom(m, r - k);
    return term1 + term2;
}
