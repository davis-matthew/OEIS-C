/*
 * A172134  Number of ways to place 3 nonattacking knights on an n X n board.
 * https://oeis.org/A172134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n+5)*(n^4 - 3*n^3 - 8*n^2 + 66*n - 108)/6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          for n >= 4.  a(1)=0, a(2)=4, a(3)=36. */

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 1) return 0;
    if (n == 2) return 4;
    if (n == 3) return 36;

    /* convert the index to seq_t once to avoid repeated casts */
    seq_t tn = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;

    /* polynomial part n^4 - 3*n^3 - 8*n^2 + 66*n - 108 */
    seq_t poly = n4 - (seq_t)3 * n3 - (seq_t)8 * n2 + (seq_t)66 * tn - (seq_t)108;

    /* (n-2)*(n+5) part */
    seq_t term1 = tn - (seq_t)2;
    seq_t term2 = tn + (seq_t)5;

    /* multiply everything and divide by 6 */
    seq_t result = (term1 * term2 * poly) / (seq_t)6;

    return result;
}
