/*
 * A168564  a(n) = n^6*(n^4 + 1)/2.
 * https://oeis.org/A168564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n^4 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 7643,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn == 0) return 0;

    seq_t n2 = nn * nn;           /* n^2   */
    seq_t n4 = n2 * n2;           /* n^4   */

    seq_t res;
    if ((nn & 1) == 0) {
        /* n even:  (n^6/2) * (n^4 + 1) */
        seq_t n2_half = n2 >> 1;          /* n^2 / 2          */
        seq_t n6_half = n4 * n2_half;     /* n^6 / 2          */
        res = n6_half * (n4 + (seq_t)1);   /* final product    */
    } else {
        /* n odd:   n^6 * ((n^4 + 1)/2) */
        seq_t n4_plus1_half = (n4 + (seq_t)1) >> 1; /* (n^4 + 1)/2   */
        seq_t n6 = n4 * n2;                 /* n^6             */
        res = n6 * n4_plus1_half;           /* final product   */
    }

    return res;
}
