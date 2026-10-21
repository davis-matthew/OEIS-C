/*
 * A052677  Expansion of e.g.f. (1-x)/(1-4*x+x^2).
 * https://oeis.org/A052677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   mul_idx_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3,
   for n ≥ 0    a(n+2) = 4*(n+2)*a(n+1) – (n+2)*(n+1)*a(n) */

#include <stddef.h>
#include "../utils/mul_idx_seq.h"

#ifdef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(n-2)  */
    seq_t prev1 = (seq_t)3;   /* a(n-1)  */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 4*i*prev1 */
        seq_t term1 = mul_idx_seq(4 * i, prev1);
        /* term2 = i*(i-1)*prev2 */
        seq_t term2 = mul_idx_seq(i * (i - 1), prev2);
        cur = term1 - term2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
