/*
 * A070038  a(n) = sum of divisors of n that are at least sqrt(n).
 * https://oeis.org/A070038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of n that are at least sqrt(n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for out‑of‑bounds calls */
    seq_t sum = 0;
    /* floor square root of n, using an arbitrary‑precision routine
       that works for all seq_t sizes. */
    uint64_t r = (uint64_t)_isqrt_seq((seq_t)n);

    for (uint64_t d = 1; d <= r; ++d) {
        if ((n % d) == 0) {               /* d divides n */
            seq_t big = (seq_t)n / (seq_t)d; /* complementary divisor >= sqrt(n) */
            sum += big;
        }
    }
    return sum;
}
