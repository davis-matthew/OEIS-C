/*
 * A007856  Subtrees in rooted plane trees on n nodes.
 * https://oeis.org/A007856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(k-1) * binomial(2n-2, n-k), where C(m) =
 *            A000108(m) is the Catalan number (equivalent to a(n) =
 *            ((n-1)/2)*C(n-1)*(1 - hypergeom([-1/2,-n],[n-1],-4)))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   a7856_gcd, a7856_mulfrac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a7856_gcd.h"
#include "../utils/a7856_mulfrac.h"

seq_t a(idx_t n) {
    uint64_t N = 2 * (uint64_t)n - 2;
    seq_t B = 1;
    uint64_t j;
    for (j = 0; j + 1 < (uint64_t)n; j++)
        B = a7856_mulfrac(B, N - j, j + 1);     /* B = binomial(N, n-1) */

    seq_t sum = 0;
    seq_t cat = 1;                               /* Catalan(k-1) */
    uint64_t k;
    for (k = 1; k <= (uint64_t)n; k++) {
        sum += cat * B;
        if (k < (uint64_t)n) {
            uint64_t i = (uint64_t)n - k;        /* B = binomial(N, i) */
            cat = a7856_mulfrac(cat, 2 * (2 * k - 1), k + 1);
            B = a7856_mulfrac(B, i, N - i + 1);  /* -> binomial(N, i-1) */
        }
    }
    return sum;
}
