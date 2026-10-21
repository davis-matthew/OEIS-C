/*
 * A053520  Denominators of successive convergents to continued fraction
 *          1/(2+2/(3+3/(4+4/(5+5/(6+6/(7+7/(8+8/9+...))))))).
 * https://oeis.org/A053520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator of the reduced fraction 1 / X₂, where X_k
 *            satisfies X_k = k + k / X_{k+1} and X_{n+2} = n+2. Recurrence:
 *            let p_k/q_k = X_k. Set p_{n+2}=n+2, q_{n+2}=1. For k from n+1
 *            down to 2: p_k = k * (p_{k+1} + q_{k+1}); q_k = p_{k+1}. Finally
 *            a(n) = p₂ / gcd(p₂, q₂)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* n >= 0 */
    idx_t N = n + 2;                 /* deepest term index */
    seq_t p = (seq_t)N;              /* p_k for current k (starts at N) */
    seq_t q = (seq_t)1;              /* q_k for current k */

    for (idx_t k = N - 1; k > 1; --k) {
        seq_t new_p = (seq_t)k * (p + q);
        seq_t new_q = p;
        p = new_p;
        q = new_q;
    }

    /* Now p/q = X₂.  The reduced reciprocal has denominator p / gcd(p,q). */
    seq_t g = seq_gcd(p, q);
    return p / g;
}
