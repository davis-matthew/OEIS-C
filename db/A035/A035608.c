/*
 * A035608  Expansion of g.f. x*(1 + 3*x)/((1 + x)*(1 - x)^3).
 * https://oeis.org/A035608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n + floor(n/2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t nn = (seq_t)n;
    /* floor(n/2) is just integer division for unsigned types */
    return nn * nn + (seq_t)(n / 2);
}
