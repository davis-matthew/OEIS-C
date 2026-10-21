/*
 * A083886  Expansion of e.g.f. exp(3*x)*exp(x^2).
 * https://oeis.org/A083886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=3*a(n-1)+2*(n-1)*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)3 + prev2 * (seq_t)(2 * (i - 1));
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
