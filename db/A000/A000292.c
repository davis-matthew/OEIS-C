/*
 * A000292  Tetrahedral (or triangular pyramidal) numbers: a(n) = C(n+2,3) =
 *          n*(n+1)*(n+2)/6.
 * https://oeis.org/A000292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, changed
 * Formula:   a(n) = C(n+2,3) = n*(n+1)*(n+2)/6
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 2952, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t f[3];
    int i;
    seq_t r;

    f[0] = n;
    f[1] = n + 1;
    f[2] = n + 2;

    /* divide out 2 from the even factor and 3 from the multiple of 3,
       so the running product never exceeds the final value */
    for (i = 0; i < 3; i++) {
        if (f[i] % 2 == 0) { f[i] /= 2; break; }
    }
    for (i = 0; i < 3; i++) {
        if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }

    r = (seq_t)f[0];
    r *= (seq_t)f[1];
    r *= (seq_t)f[2];
    return r;
}
