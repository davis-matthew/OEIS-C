/*
 * A157072  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 46.
 * https://oeis.org/A157072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..min(n,23)} binomial(n+1,i) * binomial(22,i-1) *
 *            binomial(n-i+23,23)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
/* Compute A157072 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside offset, safe value */
    idx_t limit = (n < 23) ? n : 23;
    seq_t total = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t term = binomial(n + 1, i);
        term *= binomial(22, i - 1);
        term *= binomial(n - i + 23, 23);
        total += term;
    }
    return total;
}
