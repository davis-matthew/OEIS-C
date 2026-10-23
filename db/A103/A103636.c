/*
 * A103636  a(n) = Sum_{d|n, d==0 mod 3} d^2.
 * https://oeis.org/A103636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d≡0 (mod 3)} d^2 Equivalently, if n%3==0: a(n)
 *            = 9 * sigma2(n/3), where sigma2(k) = Sum_{e|k} e^2 (sum of
 *            squares of divisors)
 * Range:     u8 n <= 17, u16 n <= 215, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   sigma2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma2_uint64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n % 3 != 0)                     /* terms not divisible by 3 are zero */
        return 0;

    idx_t k = n / 3;
    seq_t sigma = sigma2_uint64(k);      /* sum of squares of divisors of k */
    seq_t nine = 9;
    return sigma * nine;                 /* 9 * sigma2(k) = a(n) */
}
