/*
 * A062771  Order of automorphism group of the group C_n X C_2 (where C_n is
 *          the cyclic group with n elements).
 * https://oeis.org/A062771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = φ(n) if n is odd, a(n) = 4·φ(n) if n mod 4 = 0, a(n) =
 *            6·φ(n) if n mod 4 = 2
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1) */

#include "../utils/phi.h"   /* provides seq_t phi(idx_t n) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence is offset 1 */

    seq_t val = phi(n);            /* Euler's totient function */

    if ((n & 1) == 0) {            /* n is even */
        if ((n & 3) == 0)          /* n mod 4 == 0 */
            val *= 4;
        else                       /* n mod 4 == 2 */
            val *= 6;
    }
    return val;
}
