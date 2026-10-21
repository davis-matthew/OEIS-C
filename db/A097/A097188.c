/*
 * A097188  G.f. A(x) satisfies A057083(x*A(x)) = A(x) and so equals the ratio
 *          of the g.f.s of any two adjacent diagonals of triangle A097186.
 * https://oeis.org/A097188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   a(n)=3*(3*n-1)/(n+1) * a(n-1)   for n>=1   */

/* We use gcd() from utils/gcd.h to reduce intermediate
   overflow while keeping the calculation exact. */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t val = 1;   /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* numerator: 3*(3*i-1)   denominator: i+1   */
        seq_t num   = (seq_t)3 * ((seq_t)3 * i - 1);
        seq_t denom = (seq_t)i + 1;

        /* Reduce val with denom */
        seq_t g = gcd(val, denom);
        if (g != 0) {
            val   = val / g;
            denom = denom / g;
        }

        /* Reduce num with remaining denom */
        g = gcd(num, denom);
        if (g != 0) {
            num   = num / g;
            denom = denom / g;
        }

        /* After reductions denom should be 1 for exact division */
        /* Multiply val by the remaining numerator */
        val = val * num;

        /* Final division (just in case denom>1 due to rounding) */
        if (denom != 1)
            val = val / denom;
    }

    return val;
}
