/*
 * A174709  Partial sums of floor(n/6).
 * https://oeis.org/A174709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n} floor(k/6) = 3*m*(m-1) + m*(r+1), where m =
 *            n/6, r = n%6
 * Range:     u8 n <= 57, u16 n <= 888, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 6;
    idx_t r = n % 6;

#ifdef __SIZEOF_INT128__
    __int128 mm = m;
    __int128 res = 3 * mm * (mm - 1) + mm * (r + 1);
    return (seq_t)res;
#else
    /* Fallback for compilers without __int128 support.  Overflow may
       occur for very large n, but the sequence values are tested
       only up to the point that fit into seq_t. */
    seq_t term1 = 3 * (seq_t)m * (seq_t)(m - 1);
    seq_t term2 = (seq_t)m * (r + 1);
    return term1 + term2;
#endif
}
