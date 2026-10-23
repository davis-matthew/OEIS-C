/*
 * A119310  Alphabetical value of n in its Roman numerals-based
 *          representation.
 * https://oeis.org/A119310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn
 * Formula:   a(n) = sum of alphabetical positions of the Roman numeral
 *            representation of n
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */
    static const int values[13] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    static const int sums[13]   = {13,16,4,7,3,27,12,36,24,33,22,31,9};
    seq_t total = 0;
    idx_t rem = n;
    for (int i = 0; i < 13; ++i)
    {
        while (rem >= values[i])
        {
            total += sums[i];
            rem   -= values[i];
        }
    }
    return total;
}
