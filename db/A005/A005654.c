/*
 * A005654  Number of bracelets (turn over necklaces) with n red, 1 pink and
 *          n-1 blue beads; also reversible strings with n red and n-1 blue
 *          beads; also next-to-central column in Losanitsch's triangle
 *          A034851.
 * https://oeis.org/A005654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/2) * (binomial(2*n-1, n) + binomial(n-1, floor(n/2))),
 *            evaluated as a(n) = C(2n-3,n-2) + (C(2n-2,n-2) +
 *            C(n-1,floor(n/2)))/2 (since C(2n-1,n) = C(2n-2,n-1) +
 *            C(2n-2,n-2) and C(2n-2,n-1)/2 = C(2n-3,n-2)), so every
 *            intermediate is <= a(n); a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a005654_binom, a005654_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005654_binom.h"
#include "../utils/a005654_gcd.h"

seq_t a(idx_t n)
{
    seq_t x, y, z;
    if (n <= 1) return 1;
    x = a005654_binom(2 * n - 3, n - 2);
    y = a005654_binom(2 * n - 2, n - 2);
    z = a005654_binom(n - 1, n / 2);
    return x + y / 2 + z / 2 + ((y & 1) + (z & 1)) / 2;
}
