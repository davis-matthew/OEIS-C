/*
 * A000013  Definition (1): Number of n-bead binary necklaces with beads of 2
 *          colors where the colors may be swapped but turning over is not
 *          allowed.
 * https://oeis.org/A000013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{d|n} phi(2*d)*2^(n/d) / (2*n) for n > 0, a(0) = 1.
 *            Each term is split as q_d*(2n) + r_d, so no intermediate value
 *            is larger than a(n). The remainders r_d add up to a multiple of
 *            2n
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

seq_t a(idx_t n)
{
    uint64_t N, d, rsum;
    seq_t total;

    if (n == 0)
        return (seq_t)1;

    N = 2 * (uint64_t)n;
    total = 0;
    rsum = 0;

    for (d = 1; d <= (uint64_t)n; d++) {
        uint64_t c, m, i, r;
        seq_t q;
        if ((uint64_t)n % d != 0)
            continue;
        c = totient_u64(2 * d);
        m = (uint64_t)n / d;
        /* compute floor(c * 2^m / N) and its remainder by binary long division */
        q = (seq_t)(c / N);
        r = c % N;
        for (i = 0; i < m; i++) {
            q += q;
            r <<= 1;
            if (r >= N) {
                r -= N;
                q += 1;
            }
        }
        total += q;
        rsum += r;
    }
    total += (seq_t)(rsum / N);
    return total;
}
