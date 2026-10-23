/*
 * A168384  a(n) = 4*n - 2*(-1)^n.
 * https://oeis.org/A168384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2*(-1)^n
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n >= 1, 4*n >= 4, subtracting 2 is safe for unsigned seq_t. */
    if (n & 1) /* odd n */
        return (seq_t)4 * n + 2;
    else       /* even n */
        return (seq_t)4 * n - 2;
}
