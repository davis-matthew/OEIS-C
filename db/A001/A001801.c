/*
 * A001801  Coefficients of Legendre polynomials.
 * https://oeis.org/A001801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n-3)*binomial(n+3/2, n)*2^A011371(n+4) =
 *            (3*5*...*(2n+3)) / oddpart(n!) * 2^(s(n) + 1 - s(n+4)), s =
 *            binary weight
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   a1801_g, a1801_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1801_g.h"
#include "../utils/a1801_pop.h"

seq_t a(idx_t n) {
    seq_t cur = 3;
    for (idx_t k = 1; k <= n; k++) {
        uint64_t o = (uint64_t)k;
        while ((o & 1) == 0) o >>= 1;
        uint64_t m = 2 * (uint64_t)k + 3;
        uint64_t g = a1801_g(o, m);
        uint64_t d = o / g;
        cur /= d;
        cur *= (m / g);
    }
    int e = a1801_pop((uint64_t)n) + 1 - a1801_pop((uint64_t)n + 4);
    for (int i = 0; i < e; i++) cur *= 2;
    return cur;
}
