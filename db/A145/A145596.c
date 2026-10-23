/*
 * A145596  Triangular array of generalized Narayana numbers: T(n, k) =
 *          2*binomial(n + 1, k + 1)*binomial(n + 1, k - 1)/(n + 1).
 * https://oeis.org/A145596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, k) where * r = the smallest integer with r(r+1)/2 ≥
 *            n, * k = n – r(r‑1)/2 (1 ≤ k ≤ r), * T(r, k) = 2*binomial(r+1,
 *            k+1)*binomial(r+1, k‑1)/(r+1). * (The array is read row‑wise: 1,
 *            2,2, 3,8,3, 4,20,20,4, …)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* outside the offset – safe sentinel */

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Previous total number of elements before this row */
    idx_t prev_total = (r - 1) * r / 2;

    /* Column index (1‑based) within the row */
    idx_t k = n - prev_total;

    /* Compute T(r, k) = 2*binomial(r+1, k+1)*binomial(r+1, k-1)/(r+1) */
    seq_t b1 = binomial(r + 1, k + 1);
    seq_t b2 = binomial(r + 1, k - 1);

    seq_t result = b1 * b2;
    result *= (seq_t)2;
    result /= (seq_t)(r + 1);

    return result;
}
