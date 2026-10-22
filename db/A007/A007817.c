/*
 * A007817  Number of abstract simplicial 2-complexes on {1,2,3,...,n+4} which
 *          triangulate a Moebius band in such a way that all vertices lie on
 *          the boundary and are traversed in the order 1,2,3,... as one goes
 *          around the boundary.
 * https://oeis.org/A007817
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4^(n-1) -
 *            2*(29*n^3-77*n^2+106*n-88)*binomial(2*n-5,n-1)/((n-3)*(n+1)*(n+2))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   binom, gcd_u64, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pw2.h"
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    /* small native factors (fit easily for n up to ~1e5) */
    uint64_t f = 2 * (29 * n * n * n + 106 * n - 77 * n * n - 88);
    uint64_t d = (n - 3) * (n + 1) * (n + 2);
    uint64_t g = gcd_u64(f, d);
    f /= g;
    d /= g;
    /* now gcd(f, d) = 1 and d | f*C, so d | C exactly */
    seq_t c = binom(2 * n - 5, n - 1);
    seq_t q = c / (seq_t)d;
    seq_t second = q * (seq_t)f;
    seq_t pow4 = pw2(2 * (n - 1));
    return pow4 - second;
}
