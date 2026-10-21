/*
 * A239050  a(n) = 4*sigma(n).
 * https://oeis.org/A239050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*sigma(n)
 * Range:     u8 n <= 29, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* a(1) = 4, a(2) = 12, ... */
    if (n == 0) return 0;          /* offset sanity: not required by OEIS */
    return sigma_of(n) * 4;
}
