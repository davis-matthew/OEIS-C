/*
 * A014335  Exponential convolution of Fibonacci numbers with themselves
 *          (divided by 2).
 * https://oeis.org/A014335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 1, a(0)=0, a(1)=0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* a(0) = 0, a(1) = 0 */

    seq_t a0 = 0, a1 = 0, an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an = 2 * a1 + 4 * a0 + 1;
        a0 = a1;
        a1 = an;
    }
    return an;
}
