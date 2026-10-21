/*
 * A065450  Make an infinite chessboard from the squares in the first
 *          quadrant; sequence gives number of squares a knight can reach in n
 *          moves starting at the origin.
 * https://oeis.org/A065450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n+1)+2*n+5+3*(-1)^n)/8 for n ≥ 3 with a(0) = 1,
 *            a(1) = 2, a(2) = 10
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special cases that do not fit the general formula. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    /* For n ≥ 3 we use the closed‑form expression. */
    seq_t nn = (seq_t)n;                     /* safe casting of the index */
    seq_t term = (seq_t)14 * nn * (nn + 1);   /* 14*n*(n+1) */
    term += (seq_t)2 * nn;                   /* +2*n   */
    /* Add 5 + 3*(-1)^n: 5+3 = 8 when n is even, 5-3 = 2 when n is odd. */
    if ((n & 1) == 0) {                      /* n even */
        term += (seq_t)8;
    } else {                                 /* n odd */
        term += (seq_t)2;
    }
    return term / (seq_t)8;                   /* divide by 8 */
}
