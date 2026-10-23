/*
 * A033162  Begins with (3, 4); avoids 3-term arithmetic progressions.
 * https://oeis.org/A033162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005836(n-1) + 3, where A005836(n) is the number obtained
 *            by interpreting the binary representation of n as a base‑3
 *            number (i.e., replace each binary 1 with a 3ʰʰ)
 * Range:     u8 n <= 37, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    seq_t result = (seq_t)0;
    seq_t pow3   = (seq_t)1;
    idx_t k = n - 1;              /* use n-1 because a(1)=3 corresponds to A005836(0) */

    while (k)                     /* iterate over bits of k */
    {
        if (k & 1)
            result += pow3;       /* add contribution of this bit */
        pow3 *= (seq_t)3;         /* advance to next power of 3 */
        k >>= 1;
    }

    return result + (seq_t)3;     /* shift by +3 to obtain A033162 */
}
