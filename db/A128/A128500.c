/*
 * A128500  Numerators of partial sums for a series for Pi/(3*sqrt(3)).
 * https://oeis.org/A128500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n) where r(n) = Σ_{k=0..n}
 *            ((-1)^k)*S(k,1)/(k+1), with S(k,1) periodic [1,1,0,-1,-1,0]
 *            (period 6)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0; /* numerator of current partial sum */
    seq_t den = 1; /* denominator of current partial sum (always >0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* Sign pattern for the term: +1, -1, 0, +1, -1, 0, ... */
        int r = k % 3;          /* 0 -> +1, 1 -> -1, 2 -> 0 */
        if (r == 2)             /* skip zero terms */
            continue;

        seq_t tden = (seq_t)(k + 1); /* denominator of the added term */

        seq_t new_num, new_den;
        if (r == 0) { /* positive term (+1/(k+1)) */
            new_num = num * tden + den;
        } else {      /* negative term (-1/(k+1)) */
            /* The subtraction is safe because the running sum never
               becomes negative: for all k, num * tden >= den holds. */
            new_num = num * tden - den;
        }
        new_den = den * tden;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
    return num; /* numerator of r(n) in lowest terms */
}
