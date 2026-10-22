/*
 * A005890  Theta series of hexagonal close-packing with respect to center of
 *          triangle between two layers.
 * https://oeis.org/A005890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{i,j,k in Z} x^(9*(i*i+i*j+j*j) + 24*k*k) *
 *            (x^(6-12*(i+j)-8*k) + x^(3-3*(i+j)+16*k)); a(n) counts the
 *            exponents equal to n (direct search over a bounded box)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005890_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005890_isqrt.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    long long N = (long long)n;
    long long s = a005890_isqrt(N);
    long long B = s + 10;
    long long K = s / 4 + 4;
    long long i, j, k;

    for (i = -B; i <= B; i++) {
        for (j = -B; j <= B; j++) {
            long long quad = 9 * (i * i + i * j + j * j);
            long long l1 = quad + 6 - 12 * (i + j);
            long long l2 = quad + 3 - 3 * (i + j);
            if (l1 - 6 > N + 6 && l2 - 6 > N + 6) continue;
            for (k = -K; k <= K; k++) {
                long long qk = 24 * k * k;
                if (l1 + qk - 8 * k == N) result++;
                if (l2 + qk + 16 * k == N) result++;
            }
        }
    }
    return result;
}
