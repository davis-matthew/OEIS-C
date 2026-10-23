/*
 * A034771  Dirichlet convolution of d(n) (# of divisors) with b_n=2^(n-1).
 * https://oeis.org/A034771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} τ(d) * 2^{n/d - 1}
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   _tau_of, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/_tau_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range; harmless */
    seq_t sum = 0;
    idx_t sq = 0;
    while ((sq + 1) * (sq + 1) <= n) ++sq;  /* floor sqrt(n) */

    for (idx_t d = 1; d <= sq; ++d)
    {
        if (n % d != 0) continue;
        idx_t e1 = n / d;                     /* n/d   */
        seq_t pow1 = pow2(e1 - 1);             /* 2^{e1-1} */
        seq_t term1 = _tau_of(d) * pow1;
        sum += term1;

        idx_t d2 = n / d;
        if (d2 != d)                          /* paired divisor */
        {
            idx_t e2 = n / d2;                /* should equal d */
            seq_t pow2_ = pow2(e2 - 1);
            seq_t term2 = _tau_of(d2) * pow2_;
            sum += term2;
        }
    }
    return sum;
}
