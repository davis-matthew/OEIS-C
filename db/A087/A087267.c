/*
 * A087267  a(n) = gcd(n, pi(n)) where pi is A000720.
 * https://oeis.org/A087267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, pi(n))
 * Range:     u8 n <= 1374, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Prime counting function gives pi(n) */
    seq_t pi_val = compute_pi(n);
    /* Return greatest common divisor of n and pi(n) */
    return gcd((seq_t)n, pi_val);
}
