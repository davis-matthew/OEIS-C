/*
 * A141692  Triangle read by rows: T(n,k) = n*(binomial(n - 1, k - 1) -
 *          binomial(n - 1, k)), 0 <= k <= n.
 * https://oeis.org/A141692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, sign
 * Formula:   a(n) = r * (C(r‑1, k‑1) – C(r‑1, k)) where r = max{t | t(t+1)/2
 *            ≤ n}, k = n – r(r+1)/2 (row r has r+1 elements, indexed 0..r)
 * Range:     i8 n <= 46, i16 n <= 157, i32 n <= 572, i64 n <= 2169,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.  r satisfies
       r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    for (;; ++r) {
        idx_t next = (r + 1) * (r + 2) / 2;
        if (next > n) break;
    }

    /* Column index within the row.  */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Special case: the very first element in the table. */
    if (r == 0) return (seq_t)0;

    /* Compute the two binomial coefficients needed, taking care not
       to call binomial with invalid arguments (k < 0 or k > n). */
    seq_t binom_k = 0;
    if (k <= r - 1) binom_k = binomial(r - 1, k);

    seq_t binom_k_minus1 = 0;
    if (k > 0 && k - 1 <= r - 1) binom_k_minus1 = binomial(r - 1, k - 1);

    /* T(r,k) = r * (C(r-1,k-1) – C(r-1,k)) */
    seq_t result = (seq_t)r * (binom_k_minus1 - binom_k);
    return result;
}
