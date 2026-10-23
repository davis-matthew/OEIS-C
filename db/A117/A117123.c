/*
 * A117123  n minus the number of 0's in binary expansion of n.
 * https://oeis.org/A117123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n - (number of 0's in the binary representation of n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which is equivalent to a(n) = n - (bit_length(n) - popcount(n)) */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1, defined for n >= 1.  For safety, return 0 for n==0. */
    if (n == 0)
        return 0;

    /* Compute the bit length of n. */
    idx_t temp = n;
    unsigned bitlen = 0;
    while (temp)
    {
        ++bitlen;
        temp >>= 1;
    }

    /* Compute the popcount of n. */
    temp = n;
    unsigned pop = 0;
    while (temp)
    {
        temp &= temp - 1;   /* Remove lowest set bit */
        ++pop;
    }

    /* Number of zeros in the binary expansion. */
    unsigned zeros = bitlen - pop;

    /* Convert parts to seq_t for the final calculation. */
    seq_t xn = (seq_t)n;
    return xn - (seq_t)zeros;
}
