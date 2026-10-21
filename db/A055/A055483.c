/*
 * A055483  a(n) is the GCD of n and the reverse of n.
 * https://oeis.org/A055483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = gcd(n, rev(n))
 * Range:     u8 n <= 104, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   gcd, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t revn = (seq_t)rev(n);
    return gcd((seq_t)n, revn);
}
