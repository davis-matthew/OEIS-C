/*
 * A000596  Central factorial numbers: A008955(n,2).
 * https://oeis.org/A000596
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/360)*n*(n-1)*(n-2)*(2*n-1)*(2*n-3)*(5*n+1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 66, u64 n <= 2632,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t f[6];
    static const unsigned primes[3] = {2, 3, 5};
    static const unsigned exps[3] = {3, 2, 1};
    int i, j;
    unsigned e;
    seq_t result;

    f[0] = (uint64_t)n;
    f[1] = (uint64_t)n - 1;
    f[2] = (uint64_t)n - 2;
    f[3] = 2 * (uint64_t)n - 1;
    f[4] = 2 * (uint64_t)n - 3;
    f[5] = 5 * (uint64_t)n + 1;

    /* divide out 360 = 2^3 * 3^2 * 5 from the individual factors */
    for (i = 0; i < 3; i++) {
        for (e = 0; e < exps[i]; e++) {
            for (j = 0; j < 6; j++) {
                if (f[j] % primes[i] == 0) {
                    f[j] /= primes[i];
                    break;
                }
            }
        }
    }

    result = 1;
    for (j = 0; j < 6; j++)
        result *= (seq_t)f[j];
    return result;
}
