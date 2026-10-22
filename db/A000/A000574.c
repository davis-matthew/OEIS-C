/*
 * A000574  Coefficient of x^5 in expansion of (1 + x + x^2)^n.
 * https://oeis.org/A000574
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1, 4)*(n+12)/5 =
 *            (n+1)*n*(n-1)*(n-2)*(n+12)/120
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t f[5];
    static const idx_t primes[5] = {3, 5, 2, 2, 2};
    int i, j;
    seq_t r;

    f[0] = n + 1;
    f[1] = n;
    f[2] = n - 1;
    f[3] = n - 2;
    f[4] = n + 12;

    /* Cancel the denominator 120 = 2^3 * 3 * 5 from the factors */
    for (i = 0; i < 5; i++) {
        idx_t p = primes[i];
        for (j = 0; j < 5; j++) {
            if (f[j] % p == 0) {
                f[j] /= p;
                break;
            }
        }
    }

    r = 1;
    for (j = 0; j < 5; j++)
        r *= (seq_t)f[j];
    return r;
}
