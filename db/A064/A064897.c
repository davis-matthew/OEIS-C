/*
 * A064897  Period of continued fraction for sqrt(15)*n.
 * https://oeis.org/A064897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of continued fraction for sqrt(15)*n */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Work in 64‑bit arithmetic to keep the algorithm exact. */
    uint64_t N   = 15ull * (uint64_t)n * (uint64_t)n;   /* N = 15 n^2 */
    uint64_t a0  = isqrt(N);                            /* floor(sqrt(N)) */
    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a   = a0;
    uint64_t len = 0;

    /* Continued‑fraction algorithm for sqrt(N).  The period ends when
       the partial quotient equals 2 * a0. */
    while (1) {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        ++len;
        if (a == 2 * a0) break;
    }

    return (seq_t)len;
}
