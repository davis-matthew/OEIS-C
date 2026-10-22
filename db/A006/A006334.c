/*
 * A006334  From the enumeration of corners.
 * https://oeis.org/A006334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)^2*(n+2)^2*(n+3)^2*(n+4)*(2n+1)*(2n+3)*(2n+5)*(2n+7)/1360800,
 *            with 1360800 = 2^5*3^5*5^2*7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 101, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t f[12];
    static const uint64_t primes[4] = {2, 3, 5, 7};
    static const int exps[4] = {5, 5, 2, 1};
    seq_t result = 1;
    int i, j;

    if (n == 0) return 0;

    f[0] = n;
    f[1] = n + 1; f[2] = n + 1;
    f[3] = n + 2; f[4] = n + 2;
    f[5] = n + 3; f[6] = n + 3;
    f[7] = n + 4;
    f[8] = 2 * n + 1;
    f[9] = 2 * n + 3;
    f[10] = 2 * n + 5;
    f[11] = 2 * n + 7;

    /* cancel the denominator's prime powers against the individual factors */
    for (i = 0; i < 4; i++) {
        int e = exps[i];
        uint64_t p = primes[i];
        for (j = 0; j < 12 && e > 0; j++) {
            while (e > 0 && f[j] % p == 0) {
                f[j] /= p;
                e--;
            }
        }
    }

    for (j = 0; j < 12; j++)
        result *= (seq_t)f[j];

    return result;
}
