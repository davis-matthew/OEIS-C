/*
 * A180857  Wiener index of the Moebius ladder M(n).
 * https://oeis.org/A180857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 2*n - 2)/2 if n is even; a(n) = n*(n^2 + 2*n -
 *            1)/2 if n is odd
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, but harmless */
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t n_sq  = n_seq * n_seq;     /* n^2 */
    seq_t term  = n_sq + (seq_t)2 * n_seq;  /* n^2 + 2*n */

    /* subtract 2 for even n, 1 for odd n */
    seq_t sub = (n & 1) ? (seq_t)1 : (seq_t)2;

    /* numerator: n * (n^2 + 2*n - sub) */
    seq_t numerator = n_seq * (term - sub);

    /* final division by 2 */
    return numerator / 2;
}
