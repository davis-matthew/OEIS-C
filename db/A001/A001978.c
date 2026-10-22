/*
 * A001978  Number of partitions of 3n-1 into n nonnegative integers each no
 *          more than 6.
 * https://oeis.org/A001978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x*(1+2x+2x^2+x^3+2x^4+2x^5+x^6) /
 *            ((1-x)(1-x^2)^3(1-x^3)(1-x^5)). a(0..15) come from expanding
 *            this series; for n >= 16 we use the linear recurrence given by
 *            the denominator: sum_{k=0..15} D_k a(n-k) = 0, where D(x) =
 *            (1-x)(1-x^2)^3(1-x^3)(1-x^5)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 351, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    long long D[16];
    long long Nx[16] = {0, 1, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    long long g[16];
    int ks[6] = {1, 2, 2, 2, 3, 5};
    int i, j, k;

    for (i = 0; i < 16; i++) D[i] = 0;
    D[0] = 1;
    for (j = 0; j < 6; j++) {
        k = ks[j];
        for (i = 15; i >= k; i--) D[i] -= D[i - k];
    }

    for (i = 0; i < 16; i++) {
        g[i] = Nx[i];
        for (j = 1; j <= i; j++) g[i] -= D[j] * g[i - j];
    }

    if (n < 16) return (seq_t)(unsigned long long)g[n];

    seq_t w[16];
    for (i = 0; i < 16; i++) w[i] = (seq_t)(unsigned long long)g[i];

    for (idx_t m = 16; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1; k < 16; k++) {
            if (D[k] < 0)
                pos += (seq_t)(unsigned long long)(-D[k]) * w[(m - (idx_t)k) % 16];
            else if (D[k] > 0)
                neg += (seq_t)(unsigned long long)D[k] * w[(m - (idx_t)k) % 16];
        }
        w[m % 16] = pos - neg;
    }
    return w[n % 16];
}
