/*
 * A017147  a(n) = (8*n+6)^11.
 * https://oeis.org/A017147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+6)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 397,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 8*n+6 in a type that can hold the value.  */
    seq_t base = (seq_t)( ( (idx_t)8 * n ) + 6 );

    /* Raise to the 11th power by repeated multiplication.  */
    seq_t result = 1;
    for (int i = 0; i < 11; ++i)
        result *= base;

    return result;
}
