/*
 * A035327  Write n in binary, interchange 0's and 1's, convert back to
 *          decimal.
 * https://oeis.org/A035327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = bitwise complement of n within its binary length, with
 *            a(0) = 1. (a(n) = (2^k - 1) - n, where k = floor(log2 n) + 1 for
 *            n > 0.)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* special case */

    seq_t res = 0;   /* accumulator for the complement */
    seq_t bit = 1;   /* current power of two (2^i) */

    idx_t t = n;       /* working copy of n (64‑bit idx_t) */
    while (t)
    {
        if ((t & 1ULL) == 0)   /* if current LSB of n is 0, add the bit to the result */
            res += bit;

        t >>= 1;               /* move to next bit of n */
        bit <<= 1;             /* next power of two */
    }
    return res;
}
