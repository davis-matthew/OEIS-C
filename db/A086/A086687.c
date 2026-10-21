/*
 * A086687  a(n) = binomial(n!,n).
 * https://oeis.org/A086687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n!, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute using multiplicative formula:
   Let N = n!; then a(n) = ∏_{i=1}^n (N - n + i) / i  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute binomial coefficient C(fact, n) */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = fact - n + i;   /* (fact - n + i) is always positive */
        res = (res * numerator) / i;
    }

    return res;
}
