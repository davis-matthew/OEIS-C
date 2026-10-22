/*
 * A000912  Expansion of (sqrt(1-4x^2) - sqrt(1-4x))/(2x).
 * https://oeis.org/A000912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) if n is even, a(n) = C(n) - C((n-1)/2) if n is odd,
 *            where C(n) = A000108(n) = binomial(2n,n)/(n+1), computed via
 *            C(0) = 1, C(k+1) = C(k)*2*(2k+1)/(k+2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a912_cat, a912_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a912_cat.h"
#include "../utils/a912_gcd.h"

seq_t a(idx_t n) {
    seq_t cn = a912_cat(n);
    if (n % 2 == 0)
        return cn;
    return cn - a912_cat((n - 1) / 2);
}
