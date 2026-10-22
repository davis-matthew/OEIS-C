/*
 * A005714  Coefficient of x^6 in expansion of (1+x+x^2)^n.
 * https://oeis.org/A005714
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) + 6*binomial(n,4) + 5*binomial(n,5) +
 *            binomial(n,6)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 118, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, small_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/small_binom.h"

seq_t a(idx_t n) {
    seq_t r = small_binom(n, 3);
    r += small_binom(n, 4) * 6;
    r += small_binom(n, 5) * 5;
    r += small_binom(n, 6);
    return r;
}
