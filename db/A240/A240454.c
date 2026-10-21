/*
 * A240454  Smallest prime divisors of the palindromes with an even number of
 *          digits.
 * https://oeis.org/A240454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest prime divisor of concat_parts(n, rev(n))
 * Range:     u8 n <= 1681, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, digits, pow10, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/rev.h"            /* idx_t rev(idx_t x); */
#include "../utils/digits.h"         /* int digits(idx_t x); */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for offsets below 1 */

    int d = digits(n);                         /* number of decimal digits of n */
    seq_t ten_power = pow10(d);                 /* 10^d as seq_t */
    uint64_t val = (uint64_t)n * (uint64_t)ten_power + (uint64_t)rev(n);

    uint64_t spf = _smallest_prime_factor(val);
    return (seq_t)spf;
}
