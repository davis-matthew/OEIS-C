/*
 * A172517  Number of ways to place 2 nonattacking queens on an n X n toroidal
 *          board.
 * https://oeis.org/A172517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^2*(n-2)^2/2 if n even; a(n) = n^2*(n-1)*(n-3)/2 if n
 *            odd
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 300, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return 0;          /* a(1)=a(2)=a(3)=0 */
    seq_t nn = (seq_t)n;
    if ((n & 1U) == 0)             /* even n */
    {
        seq_t t = nn - 2;
        return nn * nn * t * t / 2;
    }
    else                           /* odd n */
    {
        return nn * nn * (nn - 1) * (nn - 3) / 2;
    }
}
