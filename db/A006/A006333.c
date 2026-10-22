/*
 * A006333  From the enumeration of corners.
 * https://oeis.org/A006333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(1 + n)^2*(2 + n)^2*(3 + n)*(1 + 2*n)*(3 + 2*n)*(5 +
 *            2*n))/7560
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 23, u64 n <= 294,
 *            u128 n <= 10000, big any n
 * Helpers:   a006333_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006333_gcd.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t f[9];
    seq_t d = 7560;
    seq_t r = 1;
    int i;

    if (n == 0) return 0;

    f[0] = nn;
    f[1] = nn + 1;
    f[2] = nn + 1;
    f[3] = nn + 2;
    f[4] = nn + 2;
    f[5] = nn + 3;
    f[6] = 2 * nn + 1;
    f[7] = 2 * nn + 3;
    f[8] = 2 * nn + 5;

    for (i = 0; i < 9 && d != 1; i++) {
        seq_t g = a006333_gcd(f[i], d);
        f[i] /= g;
        d /= g;
    }

    for (i = 0; i < 9; i++) r *= f[i];
    return r;
}
