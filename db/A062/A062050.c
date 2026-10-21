/*
 * A062050  n-th chunk consists of the numbers 1, ..., 2^n.
 * https://oeis.org/A062050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n - 2^floor(log_2(n))
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the largest power of two <= n */
    idx_t msb = 1;
    while (msb <= (n >> 1))
        msb <<= 1;

    /* a(n) = 1 + n - msb */
    return (seq_t)1 + (seq_t)n - (seq_t)msb;
}
