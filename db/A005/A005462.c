/*
 * A005462  Number of simplices in barycentric subdivision of n-simplex.
 * https://oeis.org/A005462
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Stirling2(n+2,n-2)*(n-3)!, using Stirling2(m,m-4) =
 *            C(m,5) + 25*C(m,6) + 105*C(m,7) + 105*C(m,8) with m = n+2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t m = n + 2;
    seq_t s = binom(m, 5) + (seq_t)25 * binom(m, 6)
            + (seq_t)105 * binom(m, 7) + (seq_t)105 * binom(m, 8);
    seq_t f = 1;
    for (idx_t i = 2; i + 3 <= n; i++)
        f *= (seq_t)i;
    return s * f;
}
