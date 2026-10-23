/*
 * A157638  Triangle of the elementwise product of binomial coefficients with
 *          q-binomial coefficients [n,k] for q = 2.
 * https://oeis.org/A157638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major flattening of the triangle T(r,k) =
 *            binomial(r,k) * [r choose k]_q evaluated at q = 2, where [r
 *            choose k]_q is the Gaussian binomial coefficient
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 125,
 *            u128 n <= 240, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* The loop runs O(sqrt(n)) times; for n ≤ 10000 r ≤ 141. */
    while ((r + 1) * (r + 2) / 2 <= n)
    {
        r++;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r   = row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k   = n - start;          /* column index */

    seq_t bin = binomial(r, k);     /* ordinary binomial coefficient */

    /* Gaussian (q‑binomial) coefficient at q = 2 */
    seq_t qb = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = pow2(r - k + i) - (seq_t)1;
        seq_t den = pow2(i) - (seq_t)1;   /* den > 0 */
        qb = (qb * num) / den;           /* exact division */
    }

    return bin * qb;
}
