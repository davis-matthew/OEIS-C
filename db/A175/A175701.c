/*
 * A175701  a(n) = n ^ (phi(n)+1), phi(n) = A000010(n) = Euler totient
 *          function.
 * https://oeis.org/A175701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n ^ (phi(n)+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = n raised to (phi(n)+1), using the provided φ and big_pow utilities. */
    return big_pow((seq_t)n, phi(n) + (seq_t)1);
}
