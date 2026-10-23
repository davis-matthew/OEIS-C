/*
 * A194487  Number of ways to arrange 3 nonattacking knights on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * (n^6 + 3*n^5 - 51*n^4 + 133*n^3 + 866*n^2 - 5944*n +
 *            10464) / 48 * for n > 4, with a(1)=0, a(2)=1, a(3)=12, a(4)=62
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 76, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)62;

    /* compute the polynomial using a 128‑bit intermediate type */
    /* all arithmetic is done in signed __int128 to avoid overflow */
    typedef __int128 i128;
    i128 nn = (i128)n;
    i128 n2  = nn * nn;
    i128 n3  = n2 * nn;
    i128 n4  = n3 * nn;
    i128 n5  = n4 * nn;
    i128 n6  = n5 * nn;

    i128 num = n6
             + (i128)3 * n5
             - (i128)51 * n4
             + (i128)133 * n3
             + (i128)866 * n2
             - (i128)5944 * nn
             + (i128)10464;

    seq_t result = (seq_t)(num / 48);
    return result;
}
