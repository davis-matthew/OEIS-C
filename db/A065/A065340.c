/*
 * A065340  Third diagonal of triangle in A046740.
 * https://oeis.org/A065340
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n-(3*n+1)*2^(n-1)+2*n^2-2*n+1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 4; for smaller n return 0 (offset only) */
    if (n < 4)
        return 0;

    /* 3^n */
    seq_t pow3n = pow3(n);

    /* 2^(n-1) */
    seq_t pow2n1 = pow2(n - 1);

    /* (3*n+1) * 2^(n-1) */
    seq_t term2 = ((seq_t)(3 * n + 1)) * pow2n1;

    /* 2*n^2 - 2*n + 1 */
    seq_t N = (seq_t)n;
    seq_t term3 = 2 * (N * N) - 2 * N + 1;

    /* a(n) = (3^n + 2*n^2 - 2*n + 1) - (3*n+1)*2^(n-1) */
    seq_t sum = pow3n + term3;
    return sum - term2;
}
