/*
 * A183208  Iterates of f(x)=floor((3x-1)/2) from x=6.
 * https://oeis.org/A183208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3 a(n-1)-1)/2) with a(1)=6
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 6, a(n) = floor((3 a(n-1)-1)/2) for n>1 */
    if (n == 1) return (seq_t)6;

    seq_t x = (seq_t)6;
    for (idx_t i = 1; i < n; ++i)
    {
        /* next = x + floor((x-1)/2) avoids overflow of 3*x */
        seq_t y = (x - 1) / 2;
        x = x + y;
    }
    return x;
}
