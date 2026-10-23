/*
 * A048018  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(b, a/2)} C(a,2k)·C(b,k) * where a =
 *            ⌊(n-2)/3⌋, b = n – a
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t a_val = (n - 2) / 3;   /* floor((n-2)/3) */
    idx_t b_val = n - a_val;     /* remaining elements */
    idx_t max_k = a_val / 2;     /* k ≤ a/2 */
    if (max_k > b_val) max_k = b_val;  /* also k ≤ b */

    seq_t sum = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t ca = binom(a_val, k << 1); /* C(a, 2k) */
        seq_t cb = binom(b_val, k);      /* C(b, k) */
        sum += ca * cb;
    }
    return sum;
}
