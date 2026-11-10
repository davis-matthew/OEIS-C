/*
 * A080352  Partial sums of A080343.
 * https://oeis.org/A080352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A080343(k) = floor((M-1)^2/4) + max(0, n -
 *            M(M+1)/2), where M = floor(sqrt(2n)); a(0) = 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A080352_fsqrt(seq_t x)
{
    if (x < 2) return x;
    seq_t lo = 1, hi = x; /* invariant: lo*lo <= x, answer in [lo,hi] */
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t M = A080352_fsqrt(N + N);
    if (M == 0) return (seq_t)0;
    seq_t j = M - 1;
    seq_t res = (j / 2) * ((j + 1) / 2); /* floor(j^2/4) */
    seq_t T = M * (M + 1) / 2;
    if (N > T) res += N - T;
    return res;
}
