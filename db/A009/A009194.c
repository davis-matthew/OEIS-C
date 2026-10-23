/*
 * A009194  a(n) = gcd(n, sigma(n)).
 * https://oeis.org/A009194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(n))
 * Range:     u8 n <= 494, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for safety 0 returns 0 */
    if (n == 0)
        return 0;
    seq_t s = sigma_of(n);
    return gcd((seq_t)n, s);
}
