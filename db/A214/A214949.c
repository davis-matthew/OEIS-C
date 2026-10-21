/*
 * A214949  Numerator of sum of reciprocals of all nonzero digits of n in
 *          decimal representation.
 * https://oeis.org/A214949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   not stated
 * Range:     u8 n <= 1377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = numerator of sum of reciprocals of all nonzero digits of n in decimal representation
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case: n == 0 has no nonzero digits, sum is 0 */
    if (n == 0)
        return 0;

    seq_t numer = 0;   /* current numerator of the sum */
    seq_t denom = 1;   /* current denominator of the sum */

    idx_t tmp = n;
    while (tmp > 0) {
        unsigned int d = tmp % 10;
        tmp /= 10;
        if (d == 0)
            continue;

        seq_t d_seq = (seq_t)d;
        seq_t new_numer = numer * d_seq + denom;
        seq_t new_denom = denom * d_seq;
        seq_t g = gcd(new_numer, new_denom);

        numer = new_numer / g;
        denom = new_denom / g;
    }

    return numer;
}
