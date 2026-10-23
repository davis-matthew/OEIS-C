/*
 * A199968  a(n) = the smallest non-divisor h of n (1<h<n), or 0 if no such h
 *          exists.
 * https://oeis.org/A199968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer h with 1 < h < n that does not
 *            divide n, or 0 if none
 * Range:     u8 n <= 2135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    seq_t nseq = (seq_t)n;
    for (idx_t h = 2; h < n; ++h)
    {
        if (nseq % (seq_t)h != 0)
            return (seq_t)h;
    }
    return (seq_t)0;
}
