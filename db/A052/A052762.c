/*
 * A052762  Products of 4 consecutive integers: a(n) = n*(n-1)*(n-2)*(n-3).
 * https://oeis.org/A052762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3) (with a(0)=0 for n<4)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All arithmetic is performed in seq_t to avoid overflows for larger builds. */
    if (n < 4) return 0;
    seq_t sn = (seq_t)n;
    return sn * (sn - 1) * (sn - 2) * (sn - 3);
}
