/*
 * A175699  a(n) = n ^ phi(n-1), phi(n) = A000010(n) = Euler totient function.
 * https://oeis.org/A175699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^phi(n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t phi_val = phi((idx_t)(n - 1));
    return big_pow((seq_t)n, phi_val);
}
