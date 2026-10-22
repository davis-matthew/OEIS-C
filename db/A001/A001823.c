/*
 * A001823  Central factorial numbers: column 2 in triangle A008956.
 * https://oeis.org/A001823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(2*n+1)*(2*n-1)*(2*n-3)*(10*n+7)/90
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a1823_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a1823_g.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t m = (seq_t)n;
    seq_t f[6];
    unsigned d = 90;
    int i;
    seq_t r = 1;
    f[0] = m;
    f[1] = m - 1;
    f[2] = 2 * m + 1;
    f[3] = 2 * m - 1;
    f[4] = 2 * m - 3;
    f[5] = 10 * m + 7;
    /* cancel the denominator factor by factor so no intermediate exceeds the result */
    for (i = 0; i < 6 && d > 1; i++) {
        unsigned g = a1823_g((unsigned)(f[i] % d), d);
        if (g > 1) { f[i] /= g; d /= g; }
    }
    for (i = 0; i < 6; i++) r *= f[i];
    return r;
}
