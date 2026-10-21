/*
 * A079282  Diagonal sums of triangle A055249.
 * https://oeis.org/A079282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)*2^n + F(n+3) where F(0)=0, F(1)=1 are Fibonacci
 *            numbers. The formula holds for all n ≥ 0 (offset 0)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* special‑case n = 0 to avoid unsigned underflow in (n-1) */
    if (n == 0U)
        return (seq_t)1;

    /* compute 2^n as a left‑shift performed by repeated multiplication */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* compute Fibonacci F(n+3) iteratively */
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;            /* F(1) */
    for (idx_t i = 0; i < n + 2; ++i) { /* n+2 iterations → F(n+3) */
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    /* f1 now holds F(n+3) */

    /* result = (n-1) * 2^n + F(n+3) */
    seq_t nm1 = (seq_t)n - (seq_t)1;
    return nm1 * pow2 + f1;
}
