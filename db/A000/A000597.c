/*
 * A000597  Central factorial numbers: A008955(n,3).
 * https://oeis.org/A000597
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n-1)*(n-2)*(n-3)*n*(2*n-1)*(2*n-3)*(2*n-5)*(35*n^2+21*n+4)/45360
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 21, u64 n <= 244, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned long long f[8];
    static const unsigned long long primes[4] = {2ULL, 3ULL, 5ULL, 7ULL};
    static const int exps[4] = {4, 4, 1, 1}; /* 45360 = 2^4 * 3^4 * 5 * 7 */
    unsigned long long N = (unsigned long long)n;
    int i, j, e;
    seq_t r;

    f[0] = N;
    f[1] = N - 1;
    f[2] = N - 2;
    f[3] = N - 3;
    f[4] = 2 * N - 1;
    f[5] = 2 * N - 3;
    f[6] = 2 * N - 5;
    f[7] = 35 * N * N + 21 * N + 4;

    for (i = 0; i < 4; i++) {
        unsigned long long p = primes[i];
        for (e = 0; e < exps[i]; e++) {
            for (j = 0; j < 8; j++) {
                if (f[j] % p == 0) {
                    f[j] /= p;
                    break;
                }
            }
        }
    }

    r = 1;
    for (j = 0; j < 8; j++)
        r *= (seq_t)f[j];
    return r;
}
