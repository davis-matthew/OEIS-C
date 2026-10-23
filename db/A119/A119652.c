/*
 * A119652  Number of different values of <= n standard American coins
 *          (pennies, nickels, dimes and quarters).
 * https://oeis.org/A119652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=25*n-56 for n>4; a(1)=4, a(2)=13, a(3)=27, a(4)=46
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 4;
    if (n == 2) return 13;
    if (n == 3) return 27;
    if (n == 4) return 46;
    seq_t tn = (seq_t)n;
    return 25 * tn - 56;
}
