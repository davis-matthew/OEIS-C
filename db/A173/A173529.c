/*
 * A173529  a(n) = 1 + A053830(n-1), where A053830 is the sum of the digits of
 *          its argument in base 9.
 * https://oeis.org/A173529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + A053830(n-1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but safe guard */
    idx_t m = n - 1;
    seq_t sum = 0;
    while (m)
    {
        sum += (seq_t)(m % 9);
        m /= 9;
    }
    return sum + 1;
}
