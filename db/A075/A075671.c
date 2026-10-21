/*
 * A075671  Sum of next n 10th powers.
 * https://oeis.org/A075671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=n(n-1)/2+1 .. n(n-1)/2+n} i^10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 11, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
/* Helper: compute x^10 using only seq_t arithmetic. */
static seq_t pow10_seq_t(seq_t x)
{
    seq_t x2 = x * x;          /* x^2 */
    seq_t x4 = x2 * x2;        /* x^4 */
    seq_t x8 = x4 * x4;        /* x^8 */
    return x8 * x2;            /* x^10 = x^8 * x^2 */
}

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* not used (offset 1), but safe */
    seq_t N = (seq_t)n;
    /* start = N*(N-1)/2 + 1 */
    seq_t start = (N * (N - 1) / 2) + 1;
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = start + (seq_t)k;
        sum += pow10_seq_t(term);
    }
    return sum;
}
