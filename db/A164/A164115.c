/*
 * A164115  Expansion of (1 - x^5) / ((1 - x) * (1 - x^4)) in powers of x.
 * https://oeis.org/A164115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 for n == 0 or n%4 != 0, otherwise a(n) = 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=0 the term is 1. For n>0, check the remainder modulo 4. */
    if (n == 0)
        return (seq_t)1;
    /* Using bitwise AND is safe for unsigned integers. */
    if ((n & 3) != 0)
        return (seq_t)1;
    return (seq_t)2;
}
