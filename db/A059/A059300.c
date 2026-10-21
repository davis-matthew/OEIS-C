/*
 * A059300  Triangle of idempotent numbers binomial(n,k)*k^(n-k), version 4.
 * https://oeis.org/A059300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 98, u64 n <= 266,
 *            u128 n <= 764, big any n
 * Helpers:   binom, binomial_multiplicative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059300 – flattened triangle of idempotent numbers
 *  a(n) = T(r,k) where r and k are defined by:
 *      r = largest integer with r*(r+1)/2 ≤ n            (row 0‑based)
 *      k = n - r*(r+1)/2                                (column 0‑based)
 *  The triangle entry is
 *      T(r,k) = C(r+1,k) * (r-k+1)^k
 *  (C is the binomial coefficient.)                    */

#include "../utils/binom.h"   /* if available; otherwise we implement binom below */
#include "../utils/binomial_multiplicative.h"

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    while ((r + 1ULL) * (r + 2ULL) / 2ULL <= n)
        ++r;

    /* column index k */
    idx_t start = r * (r + 1ULL) / 2ULL;
    idx_t k = n - start;

    /* binomial part C(r+1,k) */
    seq_t bin = binomial_multiplicative(r + 1ULL, k);

    /* power part (r-k+1)^k */
    seq_t base = (seq_t)(r - k + 1ULL);
    seq_t pow  = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow *= base;

    return bin * pow;
}
