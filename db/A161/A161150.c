/*
 * A161150  a(n) = (largest odd divisor of n)*(largest power of 2 dividing
 *          (n+1)).
 * https://oeis.org/A161150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (largest odd divisor of n)*(largest power of 2 dividing
 *            (n+1))
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count trailing zeros of a 64‑bit unsigned integer */
static inline idx_t trailing_zeros(idx_t x)
{
    idx_t count = 0;
    while ((x & 1U) == 0U) {
        x >>= 1;
        ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Safety guard – the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* largest odd divisor of n */
    idx_t odd_part = n >> trailing_zeros(n);

    /* largest power of 2 dividing (n+1) */
    idx_t exp = trailing_zeros(n + 1);
    seq_t pow2 = ((seq_t)1) << exp;

    return (seq_t)odd_part * pow2;
}
