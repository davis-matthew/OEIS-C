/*
 * A175029  a(n)=2*n if n is prime, otherwise a(n)=3*n.
 * https://oeis.org/A175029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n if n is prime, otherwise a(n) = 3*n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* defined only for n >= 1, but safe guard */
    if (is_prime(n))
        return ((seq_t)n) * 2;
    else
        return ((seq_t)n) * 3;
}
