/*
 * A047659  Number of ways to place 3 nonattacking queens on an n X n board.
 * https://oeis.org/A047659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 0 if n <= 3 a(n) = n*(n-2)^2*(2n^3 - 12n^2 + 23n - 10)/12
 *            if n is even a(n) = (n-1)(n-3)*(2n^4 - 12n^3 + 25n^2 - 14n +
 *            1)/12 if n is odd
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return 0;               /* a(0)=a(1)=a(2)=a(3)=0 */

    seq_t un = (seq_t)n;

    if ((n & 1) == 0) {                 /* even n */
        seq_t nm2 = un - 2;
        seq_t c = (seq_t)2 * un * un * un;   /* 2n^3 */
        c -= (seq_t)12 * un * un;            /* -12n^2 */
        c += (seq_t)23 * un;                 /* +23n   */
        c -= 10;                             /* -10    */

        seq_t prod = un * nm2 * nm2 * c;      /* n*(n-2)^2*C */
        prod /= 12;                           /* exact division */
        return prod;
    } else {                               /* odd n */
        seq_t nm1 = un - 1;
        seq_t nm3 = un - 3;
        seq_t c = (seq_t)2 * un * un * un * un; /* 2n^4 */
        c -= (seq_t)12 * un * un * un;           /* -12n^3 */
        c += (seq_t)25 * un * un;                /* +25n^2 */
        c -= (seq_t)14 * un;                     /* -14n   */
        c += 1;                                  /* +1     */

        seq_t prod = nm1 * nm3 * c;             /* (n-1)(n-3)*C */
        prod /= 12;                              /* exact division */
        return prod;
    }
}
