/*
 * A134177  Expansion of phi(-x^3) * psi(x^4) + x * phi(-x) * psi(x^12) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A134177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2n+1), where b is multiplicative with b(3^e)=1,
 *            b(p^e)=e+1 for p≡1,11 (mod 24), b(p^e)=(e+1)(-1)^e for p≡5,7
 *            (mod 24), b(p^e)=(1+(-1)^e)/2 for p≡13,17,19,23 (mod 24). (For
 *            n≥0, a(0)=1.)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   b_of_odd, prime_factor_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/b_of_odd.h"
#include "../utils/prime_factor_contribution.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n * 2 + 1;
    return b_of_odd(m);
}
