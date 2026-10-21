/*
 * A061319  Column 4 of A061315.
 * https://oeis.org/A061319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n+4)*(n^4+2*n^3+5*n^2+4*n+36)/576
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;

    /* term1 = n*(n+1) */
    seq_t term1 = nn * (nn + (seq_t)1);

    /* term2 = n^2 + n + 4 */
    seq_t term2 = nn * nn + nn + (seq_t)4;

    /* term3 = n^4 + 2*n^3 + 5*n^2 + 4*n + 36 */
    seq_t nn2 = nn * nn;            /* n^2 */
    seq_t nn3 = nn2 * nn;           /* n^3 */
    seq_t nn4 = nn3 * nn;           /* n^4 */
    seq_t term3 = nn4;              /* n^4 */
    term3 += (seq_t)2 * nn3;        /* 2*n^3 */
    term3 += (seq_t)5 * nn2;        /* 5*n^2 */
    term3 += (seq_t)4 * nn;         /* 4*n */
    term3 += (seq_t)36;             /* +36 */

    /* product and division (exact) */
    seq_t prod = term1 * term2;
    prod *= term3;
    return prod / (seq_t)576;
}
