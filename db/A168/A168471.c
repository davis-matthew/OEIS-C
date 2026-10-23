/*
 * A168471  a(n) = n^5*(n^9 + 1)/2.
 * https://oeis.org/A168471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 24, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the result type */
    seq_t x = (seq_t)n;

    /* compute n^2, n^4, n^5 */
    seq_t n2 = x * x;         /* n^2 */
    seq_t n4 = n2 * n2;       /* n^4 */
    seq_t n5 = n4 * x;        /* n^5 */

    /* compute n^8, n^9 and (n^9 + 1) */
    seq_t n8 = n4 * n4;       /* n^8 = (n^4)^2 */
    seq_t n9 = n8 * x;        /* n^9 = n^8 * n */
    seq_t term = n9 + 1;      /* n^9 + 1 */

    /* Divide the even factor by 2 first to avoid intermediate overflow */
    if ((x & 1) == 0) {       /* n is even → n^5 is even */
        seq_t half_n5 = n5 / 2;
        return half_n5 * term;
    } else {                  /* n is odd → term is even */
        seq_t half_term = term / 2;
        return n5 * half_term;
    }
}
