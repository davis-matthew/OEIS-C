/*
 * A225784  Denominators of the sum of the reciprocals of the Collatz (3x+1)
 *          sequence beginning at n.
 * https://oeis.org/A225784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the sum of reciprocals of the Collatz
 *            sequence starting at n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    uint64_t cur = (uint64_t)n;           /* current term of Collatz seq */
    seq_t num = 0;                       /* numerator of sum so far   */
    seq_t den = 1;                       /* denominator of sum so far */

    while (1) {
        seq_t t = (seq_t)cur;            /* current term as seq_t     */
        seq_t g  = gcd(den, t);          /* gcd(den, t)               */
        seq_t u  = den / g;              /* den/g                      */
        seq_t v  = t / g;                /* t/g                        */

        /* new numerator = num * (t/g) + den/g */
        seq_t new_num = num * v + u;
        /* new denominator = (den/g) * t = u * t */
        seq_t new_den = u * t;

        /* reduce the fraction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;

        if (cur == 1)
            break;

        /* next Collatz term */
        if (cur & 1)
            cur = cur * 3 + 1;
        else
            cur = cur / 2;
    }
    return den;
}
