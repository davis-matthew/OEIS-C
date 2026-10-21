/*
 * A215673  a(1) = 1, a(2n) = a(n)+1, a(2n+1) = a(n)+a(n+1)+1.
 * https://oeis.org/A215673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n)=a(n)+1; a(2n+1)=a(n)+a(n+1)+1
 * Range:     u8 n <= 682, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)                /* a(1) */
        return (seq_t)1;

    if ((n & 1) == 0)         /* even index: a(2n) = a(n)+1 */
        return a(n >> 1) + (seq_t)1;

    /* odd index: a(2n+1) = a(n)+a(n+1)+1 */
    return a(n >> 1) + a((n >> 1) + 1) + (seq_t)1;
}
