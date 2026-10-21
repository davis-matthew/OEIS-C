/*
 * A064837  a(n) = (6*n^4 + 30*n^3 - 20*n^2 + 14)*n/30 + (n mod 2).
 * https://oeis.org/A064837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n^4 + 30*n^3 - 20*n^2 + 14)*n)/30 + (n mod 2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 114, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast the index to seq_t once to keep all arithmetic in the big type */
    seq_t N = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n4 = n3 * N;       /* n^4 */
    seq_t n5 = n4 * N;       /* n^5 */

    /* numerator of the rational part: 6*n^5 + 30*n^4 - 20*n^3 + 14*n */
    seq_t num = 6 * n5;
    num += 30 * n4;
    num += 14 * N;          /* add the 14*n term */
    num -= 20 * n3;         /* subtract 20*n^3 */

    /* divide by 30 and add the (n mod 2) correction */
    seq_t result = num / (seq_t)30;
    if (n & 1) result += 1;
    return result;
}
