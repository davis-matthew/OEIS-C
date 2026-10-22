/*
 * A006975  Negated coefficients of Chebyshev T polynomials: a(n) =
 *          -A053120(n+10, n), n >= 0.
 * https://oeis.org/A006975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(n+1)*(n+2)*(n+3)*(n+4)*(n+10)/15
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 43, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t f[5];
    long long e = -4;
    int i;
    int done3 = 0, done5 = 0;
    seq_t r = 1;
    long long k;

    f[0] = (uint64_t)n + 1;
    f[1] = (uint64_t)n + 2;
    f[2] = (uint64_t)n + 3;
    f[3] = (uint64_t)n + 4;
    f[4] = (uint64_t)n + 10;

    /* divide out 3 and 5 exactly from small native factors */
    for (i = 0; i < 5; i++) {
        if (!done3 && f[i] % 3 == 0) { f[i] /= 3; done3 = 1; }
        if (!done5 && f[i] % 5 == 0) { f[i] /= 5; done5 = 1; }
    }
    /* strip powers of two, accumulating into the exponent */
    for (i = 0; i < 5; i++) {
        while ((f[i] & 1) == 0) { f[i] >>= 1; e++; }
    }
    e += (long long)n;

    for (i = 0; i < 5; i++) r *= (seq_t)f[i];
    for (k = 0; k < e; k++) r <<= 1;
    return r;
}
