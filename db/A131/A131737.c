/*
 * A131737  Essentially even numbers followed by duplicated odd numbers.
 * https://oeis.org/A131737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0)=0, a(1)=a(2)=1, a(n)=floor((2*n-1)/3) for n > 1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return 1;
    /* Compute floor((2*n-1)/3) using seq_t arithmetic */
    seq_t sn = (seq_t)n;
    return (sn * 2 - 1) / 3;
}
