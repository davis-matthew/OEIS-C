/*
 * A070939  Length of binary representation of n.
 * https://oeis.org/A070939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = 1 + floor(log_2(n)) for n > 0, a(0) = 1 This gives the
 *            number of bits in the binary representation of n
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    unsigned int bits = 0;
    idx_t t = n;
    while (t)
    {
        ++bits;
        t >>= 1;
    }
    return (seq_t)bits;
}
