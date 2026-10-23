/*
 * A120075  Row sums of triangle A120073 (denominator triangle for H atom
 *          spectrum).
 * https://oeis.org/A120075
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1}^{n-1} Denominator( 1/k^2 - 1/n^2 ) * =
 *            Sum_{k=1}^{n-1} (k^2 * n^2) / gcd(|n^2-k^2|, k^2 * n^2) *
 *            Offset: 2 * * This implementation computes each summand directly
 *            from the * definition of the denominator of the difference of
 *            two * reciprocals of squares. The computation is O(n) pe...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 106, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behaviour below offset is irrelevant */

    seq_t sum = 0;
    seq_t sq   = (seq_t)n * (seq_t)n;          /* n^2 */
    for (idx_t k = 1; k < n; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;        /* k^2 */
        seq_t diff = sq - ksq;                  /* |n^2 - k^2|, non‑negative since n>k */
        seq_t denom_full = ksq * sq;            /* k^2 * n^2 */
        seq_t g = gcd(diff, denom_full);       /* gcd of numerator and full denominator */
        seq_t term = denom_full / g;            /* reduced denominator */
        sum += term;
    }
    return sum;
}
