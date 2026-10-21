/*
 * A219389  Numbers k such that 13*k+1 is a square.
 * https://oeis.org/A219389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(13*n-9)/4 (n odd) * = n*(13*n-4)/4 (n even)
 * Range:     u8 n <= 9, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalently a(n) = floor(n/2) * (13*floor(n/2) - 2*(-1)^n)  */
seq_t a(idx_t n)
{
    unsigned __int128 num;
    if (n & 1)                       /* n is odd */
        num = (unsigned __int128)(n - 1) *
              (((unsigned __int128)13 * n) - 9);
    else                              /* n is even */
        num = (unsigned __int128)n *
              (((unsigned __int128)13 * n) - 4);
    return (seq_t)(num / 4);
}
