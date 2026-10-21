/*
 * A093526  Numerators of even raw moments in the distribution of line lengths
 *          for lines picked at random in the unit disk.
 * https://oeis.org/A093526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((n+1)/C_{n+1}), where C_k is the * Catalan
 *            number C_k = binom(2k, k)/(k+1). Equivalently, * a(n) =
 *            binom(2(n+1), n+1) / gcd(bin(2(n+1), n+1), (n+1)(n+2))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* m = n + 1 */
    idx_t m = n + 1;

    /* D = binom(2m, m) */
    seq_t D = binom(2 * m, m);

    /* N = m * (m + 1) */
    seq_t N = (seq_t)m * (seq_t)(m + 1);

    /* g = gcd(D, N) */
    seq_t g = gcd(D, N);

    /* a(n) = D / g */
    return D / g;
}
