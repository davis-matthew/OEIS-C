/*
 * A006659  Number of closed meander systems of order n+1 with n components.
 * https://oeis.org/A006659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*binomial(2n+2, n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   a006659_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006659_gcd.h"

seq_t a(idx_t n)
{
    /* r = C(n+3+i, i) for i = 1..n-1, ending at C(2n+2, n-1) */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i + 1 <= n; i++) {
        seq_t num = (seq_t)(n + 3 + i);
        seq_t den = (seq_t)i;
        seq_t g = a006659_gcd(r, den);
        r /= g;
        den /= g;
        r *= num / den;
    }
    return r * 2;
}
