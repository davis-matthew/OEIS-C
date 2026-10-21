/*
 * A092790  a(n) = (n+1)*phi(n-1)/2.
 * https://oeis.org/A092790
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * phi(n-1) / 2
 * Range:     u8 n <= 23, u16 n <= 367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* We assume n >= 3 (the offset).  For safety, if n < 3 we still
       compute but the result may not be meaningful for the OEIS sequence. */
    seq_t phi = calc_phi((seq_t)(n - 1));
    seq_t n1  = (seq_t)(n + 1);
    seq_t result = n1 * phi / 2;
    return result;
}
