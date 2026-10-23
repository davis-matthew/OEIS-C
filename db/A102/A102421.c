/*
 * A102421  To get a(n), start with 2n+1, multiply by 3 and add 1 and divide
 *          out any power of 2; then multiply by 3 and subtract 1 and divide
 *          out any power of 2.
 * https://oeis.org/A102421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = oddpart(3 * oddpart(3n+2) - 1)
 * Range:     u8 n <= 28, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 3 + 2;       /* start from 3n+2 */
    /* remove all factors of 2 from x */
    while ((x & (seq_t)1) == 0) x >>= 1;
    seq_t y = x * 3 - 1;              /* 3*x - 1 */
    /* remove all factors of 2 from y */
    while ((y & (seq_t)1) == 0) y >>= 1;
    return y;
}
