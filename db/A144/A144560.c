/*
 * A144560  Ten times hexagonal numbers: 10*n*(2*n-1).
 * https://oeis.org/A144560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*n*(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Use the equivalent quadratic form 20*n^2 - 10*n to avoid
       an intermediate subtraction that could underflow for n = 0. */
    seq_t val = 20 * nn * nn - 10 * nn;
    return val;
}
