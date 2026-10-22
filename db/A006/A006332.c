/*
 * A006332  From the enumeration of corners.
 * https://oeis.org/A006332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(2*n+1)*(2*n+3)/90
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 66, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t f[6];
    int primes[4] = {2, 3, 3, 5};
    int i, j;
    seq_t r;

    f[0] = n;
    f[1] = n + 1;
    f[2] = n + 1;
    f[3] = n + 2;
    f[4] = 2 * n + 1;
    f[5] = 2 * n + 3;

    /* divide out 90 = 2*3*3*5 from the factors before multiplying */
    for (i = 0; i < 4; i++) {
        idx_t p = (idx_t)primes[i];
        for (j = 0; j < 6; j++) {
            if (f[j] % p == 0) {
                f[j] /= p;
                break;
            }
        }
    }

    r = 1;
    for (j = 0; j < 6; j++)
        r *= (seq_t)f[j];
    return r;
}
