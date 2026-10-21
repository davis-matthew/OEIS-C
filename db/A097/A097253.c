/*
 * A097253  Numbers whose set of base 7 digits is {0,6}.
 * https://oeis.org/A097253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6 * (interpretation of the binary representation of n-1
 *            as a base‑7 number with digits 0 and 1)
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */

    idx_t x = n - 1;      /* binary digits start from here              */
    seq_t b = 0;          /* accumulated value in base 7                */

    if (x == 0)           /* a(1) = 0                                   */
        return 0;

    /* find position of the most significant 1 bit in x */
    unsigned msb = 0;
    idx_t t = x;
    while ((t >>= 1) != 0)
        ++msb;

    /* process bits from most significant to least significant */
    for (int i = (int)msb; i >= 0; --i) {
        int bit = (int)((x >> i) & 1);  /* 0 or 1 */
        b = b * 7 + bit;                /* shift left in base 7 and add the bit */
    }

    return 6 * b;      /* final multiplication by 6 */
}
