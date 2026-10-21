/*
 * A059146  A hierarchical sequence (W'2{2} - see A059126).
 * https://oeis.org/A059146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3*n) = 4^{v2(n+1)}, a(3*n+1) = 2*a(3*n), a(3*n+2) =
 *            4^{v2(n+1)}, where v2(x) is the exponent of the highest power of
 *            2 dividing x. Here 4^{v2(n+1)} = (2^{v2(n+1)})^2 = A006519(n)^2
 *            as required
 * Range:     u8 n <= 44, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   v2_trailing_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v2_trailing_zeros.h"

/* Return the n-th term of A059146. */
seq_t a(idx_t n)
{
    /* k corresponds to the floor division n/3 in the formula. */
    idx_t k = n / 3;

    /* Compute e = v2(k+1).  k+1 is always > 0 for n within idx_t range. */
    unsigned int e = v2_trailing_zeros((uint64_t)(k) + 1ULL);

    /* Compute 4^e using sequential multiplication to avoid shift width limits. */
    seq_t val = 1;
    for (unsigned int i = 0; i < e; ++i)
        val *= 4;

    /* For n % 3 == 1 the term is twice the value for 3*k. */
    if ((n % 3) == 1)
        val *= 2;

    return val;
}
