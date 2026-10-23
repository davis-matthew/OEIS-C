/*
 * A033676  Largest divisor of n <= sqrt(n).
 * https://oeis.org/A033676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest divisor of n that does not exceed floor(sqrt(n))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sqrt_n = floor_sqrt(N);

    /* If n is a perfect square, sqrt_n is the answer. */
    if (sqrt_n * sqrt_n == N)
        return sqrt_n;

    /* If sqrt_n divides n (non‑square case), it is the largest divisor <= sqrt(n). */
    if (N % sqrt_n == 0)
        return sqrt_n;

    /* Otherwise search downward for the next divisor. */
    for (seq_t d = sqrt_n - 1; d >= (seq_t)1; --d)
    {
        if (N % d == 0)
            return d;
    }

    /* n is prime (or n == 1), so the largest divisor <= sqrt(n) is 1. */
    return (seq_t)1;
}
