/*
 * A074742  a(n) = (n^3 + 6n^2 - n + 12)/6.
 * https://oeis.org/A074742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6*n^2 - n + 12)/6
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 2033, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;                     /* convert index to the sequence type */
    seq_t term = nn * nn * nn          /* n^3 */
              + (seq_t)6 * nn * nn     /* + 6n^2 */
              - nn                    /* - n */
              + (seq_t)12;             /* + 12 */
    return term / (seq_t)6;           /* division by 6 gives an integer */
}
