/*
 * A096893  Least area/6 of primitive Pythagorean triangles with odd leg 2n+1.
 * https://oeis.org/A096893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over coprime odd factor pairs (a,b) of N = 2n+1 of [
 *            N * ((a+b)/2) * ((a-b)/2) / 6 ] where a > b, a*b = N. This is
 *            the least area/6 of a primitive Pythagorean triangle whose odd
 *            leg is 2n+1
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* sequence is undefined below offset */

    /* N = 2n+1, fits in uint64_t */
    uint64_t N = (uint64_t)n * 2 + 1;

    seq_t best = 0;
    bool first = true;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d != 0)
            continue;
        uint64_t e = N / d;          /* e >= d */

        /* ensure both factors are odd (automatic since N odd) */
        /* check coprimality */
        if (gcd((seq_t)d, (seq_t)e) != (seq_t)1)
            continue;

        seq_t a_factor = (seq_t)e;
        seq_t b_factor = (seq_t)d;

        seq_t u = (a_factor + b_factor) / 2;
        seq_t v = (a_factor - b_factor) / 2;

        seq_t area6 = ((seq_t)N * u * v) / 6;

        if (first || area6 < best) {
            best = area6;
            first = false;
        }
    }

    return best;
}
