/*
 * A204264  Permanent of the n-th principal submatrix of A051125.
 * https://oeis.org/A204264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n X n matrix M(i,j) = max(i,j), 1 <= i,j
 *            <= n (A051125 principal submatrix); computed by Ryser's formula:
 *            sum over nonempty column subsets S of (-1)^(n-|S|) * prod_i
 *            sum_{j in S} max(i,j). a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    int N = (int)n;
    seq_t pos = 0, neg = 0;
    long long total = 1LL << N;
    for (long long mask = 1; mask < total; mask++) {
        int cnt = 0;
        long long rs[64];
        for (int i = 0; i < N; i++) rs[i] = 0;
        for (int j = 0; j < N; j++) {
            if ((mask >> j) & 1) {
                cnt++;
                for (int i = 0; i < N; i++) {
                    rs[i] += (i > j) ? (i + 1) : (j + 1);
                }
            }
        }
        seq_t prod = 1;
        for (int i = 0; i < N; i++) {
            prod = prod * (seq_t)rs[i];
        }
        if (((N - cnt) & 1) == 0) pos += prod;
        else neg += prod;
    }
    return pos - neg;
}
