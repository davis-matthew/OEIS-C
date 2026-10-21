/*
 * A061896  Triangle of coefficients of Lucas polynomials.
 * https://oeis.org/A061896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k} a(r,k) where r is the row of the triangle and
 *            a(r,0) = 1 for r > 0, a(0,0) = 2. Recurrence: a(r,k) = a(r-1,k)
 *            + a(r-2,k-1) with a(r,0)=1 (r>0). Equivalent closed form: a(r,k)
 *            = C(r-k,k) + C(r-k-1,k-1), defined for 0 ≤ k ≤ r
 * Range:     u8 n <= 81, u16 n <= 356, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th entry (rows start with r=0). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    /* Position k in the chosen row r. */
    idx_t prev_count = r * (r + 1) / 2;
    idx_t k = n - prev_count;

    /* Special first element of the triangle. */
    if (r == 0 && k == 0)
        return (seq_t)2;

    /* All subsequent first columns are 1. */
    if (k == 0)
        return (seq_t)1;

    /* If r < 2*k the binomial coefficients are zero. */
    if (r < 2 * k)
        return (seq_t)0;

    /* Compute the two binomial terms. All arguments are non‑negative
       because r ≥ 2*k. */
    seq_t first  = binom(r - k,     k);
    seq_t second = binom(r - k - 1, k - 1);

    return first + second;
}
