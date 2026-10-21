/*
 * A058002  Number of 4 X 4 matrices with entries mod n, up to row and column
 *          permutation.
 * https://oeis.org/A058002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^16 + 12*n^12 + 36*n^10 + 67*n^8 + 160*n^6 + 204*n^4 +
 *            96*n^2) / 576
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 23, u128 n <= 288,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For builds where seq_t is 64-bit or smaller, use 128‑bit intermediate to avoid overflow.
       For builds with arbitrary‑precision seq_t, compute directly with seq_t operations. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate */
        unsigned __int128 nn = n;
        unsigned __int128 p2  = nn * nn;        /* n^2   */
        unsigned __int128 p4  = p2 * p2;        /* n^4   */
        unsigned __int128 p6  = p4 * p2;        /* n^6   */
        unsigned __int128 p8  = p4 * p4;        /* n^8   */
        unsigned __int128 p10 = p8 * p2;        /* n^10  */
        unsigned __int128 p12 = p8 * p4;        /* n^12  */
        unsigned __int128 p16 = p12 * p4;       /* n^16  */

        unsigned __int128 term =
            (unsigned __int128)96 * p2
          + (unsigned __int128)204 * p4
          + (unsigned __int128)160 * p6
          + (unsigned __int128)67  * p8
          + (unsigned __int128)36  * p10
          + (unsigned __int128)12  * p12
          + p16;

        term /= (unsigned __int128)576;
        return (seq_t)term;
    } else {
        /* arbitrary‑precision build: rely on seq_t operations */
        seq_t nn  = (seq_t)n;
        seq_t p2  = nn * nn;        /* n^2   */
        seq_t p4  = p2 * p2;        /* n^4   */
        seq_t p6  = p4 * p2;        /* n^6   */
        seq_t p8  = p4 * p4;        /* n^8   */
        seq_t p10 = p8 * p2;        /* n^10  */
        seq_t p12 = p8 * p4;        /* n^12  */
        seq_t p16 = p12 * p4;       /* n^16  */

        seq_t term =
            (seq_t)96 * p2
          + (seq_t)204 * p4
          + (seq_t)160 * p6
          + (seq_t)67  * p8
          + (seq_t)36  * p10
          + (seq_t)12  * p12
          + p16;

        term /= (seq_t)576;
        return term;
    }
}
