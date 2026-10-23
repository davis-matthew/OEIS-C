/*
 * A176889  a(2*k-1)=1, a(2*k)=2*k^2 (definition by T. M. Apostol, see
 *          References).
 * https://oeis.org/A176889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k-1) = 1, a(2*k) = 2*k^2
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)      /* odd index */
        return (seq_t)1;
    else            /* even index */
    {
        seq_t k = (seq_t)(n >> 1);
        return (seq_t)2 * k * k;
    }
}
