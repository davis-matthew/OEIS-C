/*
 * A101564  a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*[(n+1) mod (k+1) = 0]*10^k,
 *          where [] denotes the Iverson bracket.
 * https://oeis.org/A101564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*[(n+1) mod (k+1) = 0]*10^k
 *            where A010060(n) is the binary digit sum of n. The sign is
 *            positive when popcount(m-j) is even, negative when odd, with m =
 *            n+1 and j = k+1 dividing m
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_seq.h"

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    idx_t m = n + 1;                      /* m = n + 1   */
    seq_t acc = pow10_seq(m - 1);          /* term with j = m (k = n)  */

    /* Collect all proper divisors j of m (j < m).  */
    idx_t divs[2000];                     /* sufficient for n ≤ 1e4 */
    idx_t cnt = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t d1 = d;
            idx_t d2 = m / d;
            if (d1 < m) divs[cnt++] = d1;
            if (d2 < m && d2 != d1) divs[cnt++] = d2;
        }
    }

    /* Process each divisor j < m.  */
    for (idx_t idx = 0; idx < cnt; ++idx) {
        idx_t j = divs[idx];
        seq_t val = pow10_seq(j - 1);      /* 10^(j-1) */

        /* Determine sign: (-1)^popcount(m-j) */
        unsigned long long bits = m - j;
        bool odd = (__builtin_popcountll(bits) & 1) != 0;

        if (odd) {
            acc -= val;                    /* negative sign */
        } else {
            acc += val;                    /* positive sign */
        }
    }

    return acc;
}
