/*
 * A107962  a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(5*n^2 + 18*n + 15)/720.
 * https://oeis.org/A107962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(5*n^2 + 18*n + 15)/720
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 t1 = nn + 1;          /* (n+1)          */
    unsigned __int128 t2 = nn + 2;          /* (n+2)          */
    unsigned __int128 t3 = nn + 3;          /* (n+3)          */
    unsigned __int128 t4 = nn + 4;          /* (n+4)          */
    unsigned __int128 poly = 5 * nn * nn + 18 * nn + 15;
    unsigned __int128 prod = t1 * t2 * t2 * t3 * t4 * poly;
    prod /= 720;                            /* divide by 720   */
    return (seq_t)prod;
}
