/*
 * A163980  a(n) = 2*n + (-1)^n.
 * https://oeis.org/A163980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + (-1)^n
 * Range:     u8 n <= 127, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U)        /* odd n */
        return (((seq_t)n) * 2) - 1;
    else               /* even n */
        return (((seq_t)n) * 2) + 1;
}
