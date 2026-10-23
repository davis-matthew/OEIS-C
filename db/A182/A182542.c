/*
 * A182542  Second diagonal of triangle in A145879.
 * https://oeis.org/A182542
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-1) - Catalan(n) * n where Catalan(n) = C(2n,n)/(n+1)
 *            (n >= 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   catalan_n, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/catalan_n.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* values for n < 3 are undefined */

    /* 4^(n-1) = 2^(2*(n-1)) */
    seq_t pow4 = pow2(2 * (n - 1));

    seq_t catalan = catalan_n(n);
    seq_t product = catalan * (seq_t)n;

    return pow4 - product;
}
