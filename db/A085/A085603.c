/*
 * A085603  a(n) = (2n)^(2n) + 1.
 * https://oeis.org/A085603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2n)^(2n) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 2n */
    seq_t base = (seq_t)n * 2;
    /* exponent = 2n */
    idx_t exp = n * 2;

    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)               /* avoid an unnecessary square when finished */
            base *= base;
    }
    return result + 1;
}
