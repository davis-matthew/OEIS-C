/*
 * A009213  a(n) = gcd(d(n), phi(n)), where d is the number of divisors of n
 *          (A000005) and phi is Euler's totient function (A000010).
 * https://oeis.org/A009213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(d(n), phi(n))
 * Range:     u8 n <= 674, u16 n <= 9440, u32 n <= 9440, u64 n <= 9440,
 *            u128 n <= 9440, big any n
 * Helpers:   divisor_count_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* divisor count d(n) */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);
    /* Euler's totient φ(n) */
    seq_t ph = phi(n);
    /* return their greatest common divisor */
    return gcd(d, ph);
}
