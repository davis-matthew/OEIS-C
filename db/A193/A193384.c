/*
 * A193384  Norm of coefficients in g.f. C(x) that satisfies: C(x) = 1 +
 *          x/C(I*x).
 * https://oeis.org/A193384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=2. For n≥4 let k = ⌊n/4⌋. Compute b
 *            = C(2k,k)/2 (central binomial coefficient divided by 2). Then
 *            a(n) = b² · 2^(n mod 4). This matches the OEIS terms
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    idx_t k   = n / 4;                 /* floor(n/4) */
    seq_t c   = binom(2 * k, k);       /* C(2k,k) */
    seq_t b   = c / (seq_t)2;          /* b = C(2k,k)/2 */
    seq_t bsq = b * b;                 /* b² */

    idx_t r = n % 4;
    if (r == 0) return bsq;
    if (r == 1) return bsq * (seq_t)2;
    if (r == 2) return bsq * (seq_t)4;
    return bsq * (seq_t)8;
}
