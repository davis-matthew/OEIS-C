/*
 * A240501  Given circular disks of radius r in a hexagonal lattice covered by
 *          a circular disk of radius R = r*2n, if the center of the circle is
 *          chosen at the middle between two lattice points, a(n) is the
 *          number of points at which an r-circle is tangent to the R-circle.
 * https://oeis.org/A240501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ∏_{p≡1 (mod 3)} (2*e_p + 1), where n ≥ 1, k = 2*n − 1
 *            = ∏ p^e_p
 * Range:     u8 n <= 144, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 1 the function returns 0 (not part of the OEIS range). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Compute k = 2*n - 1.  k fits in 64 bits for all n in the test set. */
    uint64_t k = ((uint64_t)2 * (uint64_t)n) - 1;
    if (k == 0)   /* n == 0.5, impossible, guard anyway */
        return 0;

    seq_t product = 1;

    /* Trial division to factor k.  Only odd primes are possible. */
    uint64_t tmp = k;
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (p % 3 == 1) {
                /* For p ≡ 1 (mod 3) the factor is (2*exp + 1) */
                product *= (seq_t)(2 * exp + 1);
            }
            /* p ≡ 2 (mod 3) or p == 3 contribute a factor of 1 */
        }
    }
    /* Remaining prime factor, if any */
    if (tmp > 1) {
        /* tmp is an odd prime > 1 */
        if (tmp % 3 == 1) {
            product *= 3;           /* (2*1 + 1) = 3 */
        }
        /* else factor 1 */
    }

    /* Final result: a(n) = 2 * product */
    return product * 2;
}
