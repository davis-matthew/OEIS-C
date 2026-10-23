/*
 * A033043  Sums of distinct powers of 6.
 * https://oeis.org/A033043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i>=0} ((n >> i) & 1) * 6^i (i.e., interpret binary
 *            digits of n as base‑6 digits)
 * Range:     u8 n <= 13, u16 n <= 127, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    seq_t pow6   = 1;          /* 6^0 */
    idx_t  m = n;              /* local copy to avoid modifying the argument */

    while (m != 0)
    {
        if (m & 1)
            result += pow6;    /* add 6^i if the i-th bit is set */

        pow6 *= (seq_t)6;     /* move to the next power of 6 */
        m >>= 1;              /* next bit */
    }

    return result;
}
