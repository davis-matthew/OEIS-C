/*
 * A097368  Least term in row n of the Fibonacci regression array in A097367.
 * https://oeis.org/A097368
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min over 1<=k<=n-1 of T(n,k), where T(n,k) (A097367) is
 *            the last positive term of n, k, n-k, 2k-n, ... (d(j) = d(j-2) -
 *            d(j-1)); computed by direct search over k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9782 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t best = N;
    uint64_t k;
    if (N < 2)
        return (seq_t)0;
    for (k = 1; k < N; k++) {
        int64_t x = (int64_t)N, y = (int64_t)k;
        int64_t last = y;
        for (;;) {
            int64_t z = x - y;
            if (z <= 0)
                break;
            last = z;
            x = y;
            y = z;
        }
        if ((uint64_t)last < best) {
            best = (uint64_t)last;
            if (best == 1)
                break;
        }
    }
    return (seq_t)best;
}
