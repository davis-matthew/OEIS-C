/*
 * A095201  a(n) = A095200(n)/n.
 * https://oeis.org/A095201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A095200(n)/n, where A095200(n) = max{ k*(2n-k-1)/2 :
 *            1<=k<n, n divides k*(2n-k-1)/2 }, else 0; computed by a downward
 *            search over k
 * Range:     u8 n <= 24, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t N = (seq_t)n;
    for (idx_t k = n - 1; k >= 1; --k) {
        seq_t K = (seq_t)k;
        seq_t S = K * (N + N - K - 1) / 2;
        if (S % N == 0)
            return S / N;
    }
    return 0;
}
