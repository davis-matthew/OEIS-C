/*
 * A158221  a(n) = 169n + 1.
 * https://oeis.org/A158221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*n + 1
 * Range:     u8 n <= 1, u16 n <= 387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 169 is small enough to be expressed as an integer literal.
       Cast to seq_t before multiplication to ensure the calculation
       is performed in the correct integer type.  */
    return ( (seq_t)169 * (seq_t)n ) + (seq_t)1;
}
