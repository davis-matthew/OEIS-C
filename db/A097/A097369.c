/*
 * A097369  Position in row n of Fibonacci regression array (A097367) where
 *          the least term first occurs.
 * https://oeis.org/A097369
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k in 1..n-1 minimizing T(n,k), where T(n,k) is the
 *            last positive term of n, k, n-k, 2k-n, ... (A097367); computed
 *            by direct search over k
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A097369_T(int64_t n, int64_t k)
{
    int64_t d1 = n, d2 = k, last = k;
    while (1) {
        int64_t d3 = d1 - d2;
        if (d3 <= 0) break;
        last = d3;
        d1 = d2;
        d2 = d3;
    }
    return last;
}

seq_t a(idx_t n)
{
    int64_t best = -1, bestk = 1;
    int64_t nn = (int64_t)n;
    for (int64_t k = 1; k < nn; k++) {
        int64_t t = A097369_T(nn, k);
        if (best < 0 || t < best) {
            best = t;
            bestk = k;
        }
    }
    return (seq_t)bestk;
}
