/*
 * A005508  Number of unrooted triangulations with reflection symmetry of a
 *          disk with one internal node and n+3 nodes on the boundary.
 * https://oeis.org/A005508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with C(m) = binomial(2m,m)/(m+1) (Catalan numbers), a(2m) =
 *            C(m)*(7m+2)/(m+2), a(2m-1) = 6m*C(m)/(m+2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 781 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* x * num / den, exact, where den divides x*num; after reducing num/den,
   the reduced den divides x, so no intermediate exceeds the result */
static seq_t muldiv(seq_t x, uint64_t num, uint64_t den) {
    uint64_t g = gcd_u64(num, den);
    num /= g;
    den /= g;
    if (den > 1)
        x /= (seq_t)den;
    return x * (seq_t)num;
}

static seq_t catalan(uint64_t m) {
    seq_t c = 1;
    uint64_t k;
    for (k = 0; k < m; k++)
        c = muldiv(c, 2 * (2 * k + 1), k + 2);
    return c;
}

seq_t a(idx_t n) {
    uint64_t m;
    seq_t c;
    if (n % 2 == 0) {
        m = n / 2;
        c = catalan(m);
        return muldiv(c, 7 * m + 2, m + 2);
    } else {
        m = (n + 1) / 2;
        c = catalan(m);
        return muldiv(c, 6 * m, m + 2);
    }
}
