/*
 * A086767  Last coefficient of the last term in the numerator of the
 *          simplified expansion of the solutions of FLT for n=2 for FLT
 *          n=1,2,3,..
 * https://oeis.org/A086767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, uned
 * Formula:   a(n) = 1 if n is even, otherwise a(n) = odd part of (n+1)/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even, the answer is always 1. */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* For odd n, compute m = (n+1)/2 and then strip powers of two. */
    seq_t m = ((seq_t)n + 1) / 2;      /* exact integer division because n is odd */
    while ((m & (seq_t)1) == 0)        /* while m is even */
        m >>= 1;                       /* divide by 2 */
    return m;
}
