/*
 * A138722  Concatenation of p digits 1, p digits 0 and p digits 1, where p is
 *          the n-th prime.
 * https://oeis.org/A138722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(1^p(n), 0^p(n), 1^p(n))
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* nth prime */
    idx_t p = get_prime(n);

    /* value of a block of p ones:   111...1 = (10^p – 1) / 9 */
    seq_t ones = (pow10(p) - 1) / 9;

    /* 10^(2p) */
    seq_t pow10_2p = pow10(2 * p);

    /* concatenate: ones followed by zeros followed by ones
       -> ones * 10^(2p) + ones  */
    return ones * pow10_2p + ones;
}
