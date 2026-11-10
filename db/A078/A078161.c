/*
 * A078161  a(n) = A051201(2^n).
 * https://oeis.org/A078161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051201(2^n), where A051201(N) =
 *            Sum_{i=1..floor(sqrt(N))} floor(N/i) + t*(t+1)/2 with t =
 *            floor(N/(floor(sqrt(N))+1))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-07)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

static seq_t A051201_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = isqrt(n);
    seq_t sum = 0;

    for (idx_t i = 1; i <= m; ++i) {
        sum += (seq_t)(n / i);
    }

    idx_t t = n / (m + 1);
    seq_t tt = (seq_t)t;
    sum += tt * (tt + 1) / 2;

    return sum;
}

seq_t a(idx_t n)
{
    if (n >= 64) return 0; /* 2^n does not fit in idx_t */
    idx_t N = ((idx_t)1) << n;
    return A051201_a(N);
}
