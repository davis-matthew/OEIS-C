/*
 * A071235  a(n) = (n^12 + n^6)/2.
 * https://oeis.org/A071235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 + n^6)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1722,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is performed as (n^6 * (n^6 + 1))/2, 
   splitting the even factor first to avoid overflow. */

seq_t a(idx_t n) {
    seq_t s   = (seq_t)n;
    seq_t n2  = s * s;          /* n^2   */
    seq_t n4  = n2 * n2;        /* n^4   */
    seq_t n6  = n4 * n2;        /* n^6   */
    seq_t n6p1 = n6 + 1;        /* n^6 + 1 */

    /* Divide the even factor by 2 beforehand */
    if ((n6 & 1) == 0) {        /* n^6 is even */
        return (n6 / 2) * n6p1;
    } else {                    /* n^6 + 1 is even */
        return n6 * (n6p1 / 2);
    }
}
