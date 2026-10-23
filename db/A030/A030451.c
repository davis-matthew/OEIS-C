/*
 * A030451  a(2*n) = n, a(2*n+1) = n+2.
 * https://oeis.org/A030451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = n, a(2*n+1) = n+2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* safe cast from idx_t to seq_t */
    if ((nn & 1) == 0) {
        return nn >> 1;        /* even: a(n) = n/2 */
    } else {
        return (nn + 3) >> 1;  /* odd:  a(n) = (n+3)/2 = (n-1)/2 + 2 */
    }
}
