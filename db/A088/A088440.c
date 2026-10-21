/*
 * A088440  a(4n) = 4n, otherwise a(n) = 1.
 * https://oeis.org/A088440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 4 == 0) ? n : 1
 * Range:     u8 n <= 255, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 4) == 0)
        return (seq_t)n;
    else
        return (seq_t)1;
}
