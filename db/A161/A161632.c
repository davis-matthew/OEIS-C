/*
 * A161632  E.g.f. satisfies A(x) = (1 + x*exp(x*A(x)))^2.
 * https://oeis.org/A161632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} C(2*(n-k+1),k) * k^(n-k) /
 *            ((n-k+1)*(n-k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(n) for n>0 is computed by the explicit sum above. */
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    seq_t r   = (seq_t)1;            /* r = n!/(n-k+1)! for current k */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t a_val = 2 * (n - k + 1);
        seq_t bin = binom(a_val, k);
        if (bin == 0) {
            /* Next r = r * (n-k+1) */
            r *= (n - k + 1);
            continue;
        }
        seq_t powkk = pow_uint((uint64_t)k, n - k);  /* k^(n-k) */
        seq_t term  = bin * powkk * r;
        sum += term;
        r *= (n - k + 1);   /* prepare r for next k */
    }
    return sum;
}
