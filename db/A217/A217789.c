/*
 * A217789  Least difference between 2 palindromic numbers of length n.
 * https://oeis.org/A217789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 for n==1; 11 for n==2; 10 for n==3; 11 for n>=4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    else if (n == 2)
        return (seq_t)11;
    else if (n == 3)
        return (seq_t)10;
    else
        return (seq_t)11;
}
