/*
 * A055143  The first n digits of the juxtaposition of the base-2 numbers
 *          converted to decimal.
 * https://oeis.org/A055143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer represented by the first n bits of the *
 *            concatenation of the binary representations of 1, 2, 3, ... *
 *            The concatenated string is interpreted as a binary number. *
 *            This function builds the number bit by bit
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* under-offset behavior is irrelevant */

    seq_t result = 0;
    idx_t bits_done = 0;

    for (idx_t k = 1; bits_done < n; ++k)
    {
        /* Determine the most significant bit position of k */
        idx_t msb = 0;
        for (idx_t t = k; t > 1; t >>= 1)
            ++msb;

        /* Process bits from msb down to 0 */
        for (int pos = (int)msb; pos >= 0 && bits_done < n; --pos)
        {
            int bit = (int)((k >> (idx_t)pos) & 1);
            result = result * 2 + bit;
            ++bits_done;
        }
    }

    return result;
}
