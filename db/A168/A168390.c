/*
 * A168390  a(n) = 1 + 8*floor(n/2).
 * https://oeis.org/A168390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8*floor(n/2)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset = 1  */

seq_t a(idx_t n)
{
    /* Compute in idx_t to avoid overflow in seq_t for small types. */
    return (seq_t)((idx_t)8 * (n / 2) + 1);
}
