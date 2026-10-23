/*
 * A143376  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in the cube Q_n of dimension n (1 <= k <=
 *          n).
 * https://oeis.org/A143376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 160, u64 n <= 576,
 *            u128 n <= 2238, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS sequence A143376.
   The triangle T(m,k) = 2^(m-1) * binomial(m,k) is read row‑wise.
   For a given n (1‑based), find the smallest m with m(m+1)/2 >= n;
   let k = n - m(m-1)/2; then a(n) = 2^(m-1) * binomial(m,k). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find row m such that m(m+1)/2 >= n. */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n)
        ++m;

    /* k is the position within the row (1‑based). */
    idx_t prev_terms = (m - 1) * m / 2;
    idx_t k = n - prev_terms;   /* 1 <= k <= m */

    seq_t pow2_val = pow2(m - 1);
    seq_t bin_val   = binomial(m, k);

    return pow2_val * bin_val;
}
