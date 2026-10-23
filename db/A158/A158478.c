/*
 * A158478  Number of colors needed to paint n sectors of a circle.
 * https://oeis.org/A158478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n mod 2) + (2 mod (n+1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned integer type.  The modulo operations use
       the usual integer promotion rules, with 2 being an int
       and n+1 being idx_t.  The result fits into seq_t. */
    return (seq_t)(n & 1) + (seq_t)(2 % (n + 1));
}
