/*
 * A069177  Maximal power of 2 that divides Phi(n), or the size of the Sylow
 *          2-subgroup of the group of units mod n.
 * https://oeis.org/A069177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^ν2(φ(n)) where ν2(x) is the exponent of 2 dividing x
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;               /* undefined for n < 1 */

    seq_t ph = phi(n);                  /* Euler's totient of n */

    idx_t v = 0;
    while ((ph & (seq_t)1) == (seq_t)0)
    {
        v++;
        ph >>= 1;
    }

    return ((seq_t)1 << v);             /* 2^v */
}
