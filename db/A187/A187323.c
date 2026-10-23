/*
 * A187323  a(n) = floor(n/2) + floor(n/3) + floor(n/4).
 * https://oeis.org/A187323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/3) + floor(n/4)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Perform integer divisions on the index type first to keep the
       intermediate results small, then cast to the target type. */
    return (seq_t)(n / 2) + (seq_t)(n / 3) + (seq_t)(n / 4);
}
