/*
 * A083542  a(n) = phi(n+1)*phi(n), product of totients of two consecutive
 *          integers.
 * https://oeis.org/A083542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n)*phi(n+1)
 * Range:     u8 n <= 27, u16 n <= 381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the precomputed Euler totient utility */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    return phi(n) * phi(n + 1);
}
