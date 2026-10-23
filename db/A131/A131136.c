/*
 * A131136  Denominator of (exponential) expansion of log((x/2-1)/(x-1)).
 * https://oeis.org/A131136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 2^(1 + popcount(n-1))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount is the number of 1‑bits in the binary expansion of n-1 */

#include <stdint.h>

/* minimal popcount for idx_t (uint64_t) */
static inline unsigned popcount_idx(idx_t x)
{
    unsigned c = 0;
    while (x)
    {
        x &= x - 1;   /* clear least significant 1‑bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    /* compute exponent: 1 + popcount(n-1) */
    unsigned exp = popcount_idx(n - 1U) + 1U;

    /* build 2^exp in seq_t (works even if exp > width of seq_t,
       as unsigned overflow is defined modulo 2^width) */
    seq_t result = (seq_t)1;
    while (exp--)
        result *= 2;

    return result;
}
