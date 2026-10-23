/*
 * A192811  a(n) = A192809(n)/2.
 * https://oeis.org/A192811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((3*n-6)/2)} binomial(3*n-4-2*k, k)*2^k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;

    seq_t sum = 0;
    idx_t kmax = (3 * n - 6) / 2;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t top = 3 * n - 4 - 2 * k;
        seq_t bin = binomial(top, k);
        seq_t powv = pow2(k);
        sum = sum + bin * powv;
    }
    return sum;
}
