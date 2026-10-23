/*
 * A154514  a(n) = 648*n^2 - 72*n + 1.
 * https://oeis.org/A154514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 648*n^2 - 72*n + 1 OEIS offset: 1 (a(1) is the first
 *            printed term)
 * Range:     u8 none, u16 n <= 10, u32 n <= 2574, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for exact arithmetic in the target type */
    seq_t tn = (seq_t)n;
    /* compute the quadratic formula using seq_t arithmetic */
    seq_t res = (seq_t)648 * tn * tn - (seq_t)72 * tn + (seq_t)1;
    return res;
}
