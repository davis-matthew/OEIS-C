/*
 * A216787  a(n) = Product_{k=1..n} (144 - 12/k).
 * https://oeis.org/A216787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (144 - 12/k) Recurrence used: a(0) = 1,
 *            a(n) = a(n-1) * (144*n - 12) / n for n ≥ 1 (Exact integer
 *            division; the numerator is always divisible by n.) This
 *            implementation evaluates the product directly, with no
 *            memoisation
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)144 * i - (seq_t)12;  /* 144*i - 12 */
        /* exact division by i */
        result = result * num / i;
    }
    return result;
}
