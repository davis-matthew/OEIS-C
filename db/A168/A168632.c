/*
 * A168632  a(n) = n^6*(n^8 + 1)/2.
 * https://oeis.org/A168632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 24, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;          /* cast index to the seq_t type */
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n6 = n4 * n2;           /* n^6 */
    seq_t n8 = n4 * n4;           /* n^8 */

    seq_t res;
    if ((ns % 2) == 0) {
        /* n is even → n^6 is even → divide n^6 by 2 first */
        seq_t n6_half = n6 / 2;
        seq_t factor   = n8 + 1;   /* n^8 + 1 */
        res = n6_half * factor;
    } else {
        /* n is odd → n^8 + 1 is even → divide that by 2 first */
        seq_t factor   = (n8 + 1) / 2;
        res = n6 * factor;
    }
    return res;
}
