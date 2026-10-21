/*
 * A224525  Number of idempotent 3 X 3 0..n matrices of rank 1.
 * https://oeis.org/A224525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n*n + 12*n + 3 + 6 * Σ_{k=1}^n ⌊n/k⌋ (OEIS A224525)
 * Range:     u8 n <= 4, u16 n <= 101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at n=1; 0 is undefined */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += (seq_t)(n / k);     /* floor division in integer types */

    seq_t nn = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t result = (seq_t)6 * nn          /* 6*n^2 */
                + (seq_t)12 * n          /* +12*n */
                + (seq_t)3
                + (seq_t)6 * sum;       /* +6*Σ floor(n/k) */

    return result;
}
