/*
 * A032807  Numbers whose set of base-7 digits is {2,3}.
 * https://oeis.org/A032807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose set of base-7 digits is {2,3}
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Map n to binary representation: n+1 in binary,
       drop the leading 1 to get the digit pattern. */
    idx_t m = n + (idx_t)1;          /* m >= 2 */
    idx_t L = 0;                    /* length of the pattern */
    idx_t temp = m;
    for (; temp > 1; ++L)           /* count bits of m */
        temp >>= 1;

    idx_t pattern = m - ((idx_t)1 << L); /* bits after the leading 1 */

    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        idx_t shift = L - 1 - i;
        idx_t bit   = (pattern >> shift) & (idx_t)1;
        seq_t digit = (seq_t)(2 + bit);   /* 0 -> 2, 1 -> 3 */
        result = result * ((seq_t)7) + digit;
    }
    return result;
}
