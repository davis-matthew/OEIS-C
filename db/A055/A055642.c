/*
 * A055642  Number of digits in the decimal expansion of n.
 * https://oeis.org/A055642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(0)=1; for n>0, a(n)=1+floor(log10(n)) – number of decimal
 *            digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* special case: 0 has one digit */
    seq_t digits = 1;
    /* Count digits by repeated division by 10; at most 20 iterations for uint64_t. */
    idx_t m = n;
    while (m >= 10)
    {
        m /= 10;
        digits++;
    }
    return digits;
}
