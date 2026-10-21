/*
 * A088128  Expansion of e.g.f.: cosh(x)/(1-x)^2.
 * https://oeis.org/A088128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n, 2*k) * (n-2*k+1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute a(n) for OEIS sequence A088128.
 * idx_t is an unsigned 64‑bit integer, seq_t is an unsigned integer type
 * (8, 16, 32, 64, 128 bits or arbitrary precision).  The function uses
 * only integer operations. */
seq_t a(idx_t n)
{
    /* Sum of the terms. */
    seq_t sum = 0;

    /* Compute (n+1)! as the starting factorial value.
     * For n = 0, this is 1! = 1, which is the required value. */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n + 1; ++i)
        fact *= (seq_t)i;

    /* Iterate over k = 0 .. floor(n/2).  After each term, update
     * the factorial to (n-2(k+1)+1)! by dividing by the two
     * consecutive numbers (n-2k+1) and (n-2k).  The division is
     * exact by construction of the factorial sequence. */
    for (idx_t k = 0; ((k << 1) <= n); ++k) {
        seq_t bin = binom(n, k << 1);   /* binomial(n, 2*k)               */
        sum += bin * fact;              /* add the current term             */

        if ((k << 1) < n) {             /* prepare for the next k (if any)   */
            idx_t a1 = n - (k << 1) + 1;
            idx_t a2 = n - (k << 1);
            /* a1 and a2 are positive; their product fits in idx_t. */
            fact /= (seq_t)(a1 * a2);
        }
    }

    return sum;
}
