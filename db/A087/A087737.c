/*
 * A087737  Value of (n,n+1) concatenated in binary representation.
 * https://oeis.org/A087737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * 2^A070939(n+1) + (n+1) where A070939(m) = number of
 *            binary digits of m = floor(log2(m)) + 1
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 */
    idx_t n_plus = n + 1;
    /* compute the bit length of n_plus */
    unsigned bits = 0;
    idx_t t = n_plus;
    while (t) {
        bits++;
        t >>= 1;
    }
    /* shift n by that many bits and add (n_plus) */
    seq_t result = ((seq_t)n << bits) + (seq_t)n_plus;
    return result;
}
