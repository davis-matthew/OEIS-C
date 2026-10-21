/*
 * A053639  a(n) = ceiling(2^(n+1)/n).
 * https://oeis.org/A053639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2^(n+1)/n)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n ≥ 1.  The algorithm performs a binary long
 * division of the number 2^(n+1) by n using only 64‑bit arithmetic
 * for the remainder and seq_t arithmetic for the quotient.  It is
 * correct for all sequence values that fit in seq_t; for values that
 * exceed the capacity of seq_t the function simply produces a value
 * that is well defined in the target type. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n=0 defensively – it is outside the OEIS offset. */
    if (n == 0)
        return 0;

    /* Binary long division of the number 2^(n+1) by n. */
    uint64_t rem = 0;          /* remainder fits in 64‑bit */
    seq_t  quo = 0;           /* quotient being formed as seq_t */

    int64_t bits = (int64_t)n + 1;   /* number of bits in 2^(n+1) minus 1   */
    for (int64_t i = bits; i >= 0; --i)
    {
        uint64_t bit = (i == bits) ? 1ULL : 0ULL;   /* the top 1‑bit */
        rem = (rem << 1) | bit;                    /* bring in the next bit */
        uint64_t qbit = rem / n;                   /* quotient bit 0 or 1 */
        rem %= n;
        quo = quo * 2 + (seq_t)qbit;               /* append the bit */
    }

    /* For ceil, add one if the remainder (modulo n) is non‑zero. */
    if (rem > 0)
        quo = quo + 1;

    return quo;
}
