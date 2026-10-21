/*
 * A079632  a(n) = floor(n/floor(sqrt(n)))-floor(sqrt(n)).
 * https://oeis.org/A079632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / floor(sqrt(n))) - floor(sqrt(n))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(v)) for 0 <= v < 2^64 */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t x = v, y = (x + 1) >> 1;
    while (y < x)
    {
        x = y;
        y = (x + v / x) >> 1;
    }
    return x;
}

/* Sequence A079632: a(n) = ⌊ n / ⌊√n⌋ ⌋ – ⌊√n⌋ */
seq_t a(idx_t n)
{
    /* n >= 1 (offset 1). */
    uint64_t sq = isqrt_u64(n);          /* floor(sqrt(n)) as unsigned 64-bit */
    seq_t sqs = (seq_t)sq;              /* promote to seq_t */
    seq_t div = (seq_t)n / sqs;          /* floor(n / floor(sqrt(n))) */
    return div - sqs;                    /* subtract floor(sqrt(n)) */
}
