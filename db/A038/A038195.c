/*
 * A038195  Triangle read by rows: T(n,k) (n >= 2, 0 <= k <= n) = number of
 *          over-all crude totals of unbranched k-5-catapolyheptagons.
 * https://oeis.org/A038195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the row indexed from 2 upwards * and k
 *            is the column such that the flattened triangle index n * (offset
 *            2) satisfies cumulative entries < n-2 <= cumulative + (r+1) *
 *            with T(r,k) = 2^(2*r-k-2)*(binomial(r-1,k-1)+binomial(r-2,k)/4).
 *            * The implementation uses the provided bi...
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined below offset */

    /* Find the row r (>=2) containing the entry of index n.
     * Flat index pos (zero‑based) is n-2.  We accumulate row lengths
     * (which are r+1 for row r) until we exceed pos. */
    idx_t pos = n - 2;
    idx_t r = 2;
    idx_t cum = 0;

    while (cum + (r + 1) <= pos) {
        cum += r + 1;
        ++r;
    }

    idx_t k = pos - cum;          /* column in row r */

    /* Compute T(r,k). */
    idx_t e = 2 * r - k - 2;      /* exponent in the power of two */
    seq_t A, B, sum, result;

    /* binomial(r-1,k-1) is zero when k==0 */
    if (k == 0)
        A = 0;
    else
        A = binomial(r - 1, k - 1);

    /* binomial(r-2,k) is zero when k > r-2 anyway */
    B = binomial(r - 2, k);

    /* 4*A + B fits into seq_t for the ranges we care about. */
    sum = 4 * A + B;

    if (e >= 2) {
        /* use bit shift, shifted by e-2.  Use 128‑bit intermediate
         * to avoid overflow when seq_t is 64‑bit and the value fits. */
        __int128 tmp = (__int128)sum << (e - 2);
        result = (seq_t)tmp;
    } else {
        /* e is 0 or 1.  Compute as (2^e * sum) / 4. */
        seq_t pow2 = 1;
        if (e >= 1)
            pow2 <<= e;          /* 2^e  (e==1 -> 2) */
        result = (pow2 * sum) / 4;
    }

    return result;
}
