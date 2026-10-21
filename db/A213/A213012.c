/*
 * A213012  Trajectory of 26 under the Reverse and Add! operation carried out
 *          in base 2.
 * https://oeis.org/A213012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 26; for n > 0: a(n) = a(n-1) + reverse_bits(a(n-1)),
 *            where reverse_bits(x) is the binary reversal of x using its
 *            minimal bit-length
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse the binary representation of x (using only its significant bits). */
static seq_t reverse_bits(seq_t x)
{
    /* zero is a special case: its reversal is zero */
    if (x == 0)
        return 0;

    /* Determine the length of the significant bit sequence. */
    seq_t tmp = x;
    unsigned len = 0;
    while (tmp)
    {
        tmp >>= 1;
        ++len;
    }

    /* Build the reversed value. */
    seq_t rev = 0;
    tmp = x;
    for (unsigned i = 0; i < len; ++i)
    {
        rev <<= 1;
        rev |= (tmp & 1);
        tmp >>= 1;
    }
    return rev;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    seq_t val = 26;           /* a(0) = 26 */

    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i)
    {
        val += reverse_bits(val);
    }
    return val;
}
