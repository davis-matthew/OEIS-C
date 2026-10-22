/*
 * A005466  5 X 5 stochastic matrices of integers.
 * https://oeis.org/A005466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   let H(r) = number of 5 X 5 nonnegative integer matrices with all
 *            row and column sums r (computed by a DP over rows), and h_k =
 *            Sum_{j=0..k} (-1)^j*C(17,j)*H(k-j) (h* vector of the Birkhoff
 *            polytope B_5, Sum_r H(r) x^r = h(x)/(1-x)^17). Then h(x) =
 *            Sum_{k=0..6} a(k) x^k (1-x)^(12-2k), i.e. a(k) =...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   cnt_H, from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/from_i128.h"
#include "../utils/cnt_H.h"

static long long binom_ll(int n, int k)
{
    long long r = 1;
    int i;
    if (k < 0 || k > n) return 0;
    for (i = 1; i <= k; i++) r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n)
{
    static bool done = false;
    static long long av[7];
    if (!done) {
        long long H[7], h[7];
        int k, j;
        for (k = 0; k <= 6; k++) H[k] = cnt_H(k);
        for (k = 0; k <= 6; k++) {
            long long s = 0;
            for (j = 0; j <= k; j++) {
                long long t = binom_ll(17, j) * H[k - j];
                s += (j & 1) ? -t : t;
            }
            h[k] = s;
        }
        for (k = 0; k <= 6; k++) {
            long long s = h[k];
            for (j = 0; j < k; j++) {
                long long t = av[j] * binom_ll(12 - 2 * j, k - j);
                s -= ((k - j) & 1) ? -t : t;
            }
            av[k] = s;
        }
        done = true;
    }
    if (n > 6) return (seq_t)0;
    return from_i128((__int128)av[n]);
}
